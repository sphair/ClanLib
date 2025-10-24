/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "Sound/precomp.h"
#include "soundprovider_vorbis_session.h"
#include "soundprovider_vorbis_generic.h"
#include "API/Sound/soundformat.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_memory.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Vorbis_Session construction:

CL_SoundProvider_Vorbis_Session::CL_SoundProvider_Vorbis_Session(CL_SoundProvider_Vorbis_Generic *data) :
	data(data), num_samples(0), position(0), input(0), stream_eof(false)
{
	input = new CL_InputSource_Memory(data->buffer, data->buffer_size, false);

	ogg_sync_init(&oy); /* Now we can read pages */

	char *buffer;
	int bytes;
	
	buffer = ogg_sync_buffer(&oy,4096);
	bytes = input->read(buffer, 4096);
	ogg_sync_wrote(&oy,bytes);

	/* Get the first page. */
	if (ogg_sync_pageout(&oy,&og) !=1)
	{
		/* error case.  Must not be Vorbis data */
		throw CL_Error("Input does not appear to be an Ogg bitstream.");
	}
	
	/* Get the serial number and set up the rest of decode. */
	/* serialno first; use it to set up a logical stream */
	ogg_stream_init(&os,ogg_page_serialno(&og));

	vorbis_info_init(&vi);
	vorbis_comment_init(&vc);
	if (ogg_stream_pagein(&os,&og)<0)
	{ 
		/* error; stream version mismatch perhaps */
		throw CL_Error("Error reading first page of Ogg bitstream data.");
	}
	
	if (ogg_stream_packetout(&os,&op)!=1)
	{ 
		/* no page? must not be vorbis */
		throw CL_Error("Error reading initial header packet.");
	}
    
	if (vorbis_synthesis_headerin(&vi,&vc,&op)<0)
	{ 
		/* error case; not a vorbis header */
		throw CL_Error("This Ogg bitstream does not contain Vorbis "
		      "audio data.");
	}
    
	/* At this point, we're sure we're Vorbis.  We've set up the logical
	   (Ogg) bitstream decoder.  Get the comment and codebook headers and
	   set up the Vorbis decoder */
    
	/* The next two packets in order are the comment and codebook headers.
	   They're likely large and may span multiple pages.  Thus we reead
	   and submit data until we get our two pacakets, watching that no
	   pages are missing.  If a page is missing, error out; losing a
	   header page is the only place where missing data is fatal. */
    
	int i=0;
	while (i<2)
	{
		while (i<2)
		{
			int result=ogg_sync_pageout(&oy,&og);
			if(result==0) break; /* Need more data */
			/* Don't complain about missing or corrupt data yet.  We'll
			   catch it at the packet output phase */
			if(result==1)
			{
				ogg_stream_pagein(&os,&og);
				/* we can ignore any errors here
				   as they'll also become apparent
				   at packetout */

				while(i<2)
				{
					result=ogg_stream_packetout(&os,&op);
					if(result==0)break;
					if(result==-1)
					{
						/* Uh oh; data at some point was corrupted or missing!
						We can't tolerate that in a header.  Die. */
						throw CL_Error("Corrupt secondary header.");
					}
					vorbis_synthesis_headerin(&vi,&vc,&op);
					i++;
				}
			}
		}
		/* no harm in not checking before adding more */
		buffer=ogg_sync_buffer(&oy,4096);
		bytes=input->read(buffer,4096);
		if(bytes==0 && i<2)
		{
			throw CL_Error("End of file before finding all Vorbis headers!");
		}
		ogg_sync_wrote(&oy,bytes);
	}
    
	/* Throw the comments plus a few lines about the bitstream we're
	decoding */
/*	{
		char **ptr=vc.user_comments;
		while(*ptr)
		{
			fprintf(stderr,"%s\n",*ptr);
			++ptr;
		}
		fprintf(stderr,"\nBitstream is %d channel, %ldHz\n",vi.channels,vi.rate);
		fprintf(stderr,"Encoded by: %s\n\n",vc.vendor);
	}
    
	convsize=4096/vi.channels;
*/
//	CL_Log::log("debug", "vc.vendor: %1", vc.vendor);
	
	/* OK, got and parsed all three headers. Initialize the Vorbis
	packet->PCM decoder. */
	vorbis_synthesis_init(&vd,&vi); /* central decode state */
	vorbis_block_init(&vd,&vb);     /* local state for most of the decode
				       so multiple block decodes can
				       proceed in parallel.  We could init
				       multiple vorbis_block structures
				       for vd here */

}

CL_SoundProvider_Vorbis_Session::~CL_SoundProvider_Vorbis_Session()
{
	delete input;

	/* clean up this logical bitstream; before exit we see if we're
	followed by another [chained] */
	
	ogg_stream_clear(&os);
	
	/* ogg_page and ogg_packet structs always point to storage in
	libvorbis.  They're never freed or manipulated directly */
	
	vorbis_block_clear(&vb);
	vorbis_dsp_clear(&vd);
	vorbis_comment_clear(&vc);
	vorbis_info_clear(&vi);  /* must be called last */

	/* OK, clean up the framer */
	ogg_sync_clear(&oy);
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Vorbis_Session attributes:

int CL_SoundProvider_Vorbis_Session::get_num_samples() const
{
	return num_samples;
}

int CL_SoundProvider_Vorbis_Session::get_frequency() const
{
	return vi.rate;
}

CL_SoundFormat CL_SoundProvider_Vorbis_Session::get_format() const
{
	return sf_16bit_signed;
}

int CL_SoundProvider_Vorbis_Session::get_num_channels() const
{
	return vi.channels;
}

int CL_SoundProvider_Vorbis_Session::get_position() const
{
	return position;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Vorbis_Session operations:

bool CL_SoundProvider_Vorbis_Session::eof() const
{
	return stream_eof;
}

void CL_SoundProvider_Vorbis_Session::stop()
{
}

bool CL_SoundProvider_Vorbis_Session::play()
{
	return true;
}
	
bool CL_SoundProvider_Vorbis_Session::set_position(int pos)
{
	// Currently only support seeking to beginning of stream.
	if (pos != 0) return false;

	input->seek(0, CL_InputSource::seek_set);
	stream_eof = false;
	return true;
}

int CL_SoundProvider_Vorbis_Session::get_data(void **data_ptr, int data_requested)
{
	int data_left = data_requested;
	while (!eof() && data_left > 0)
	{
		float **pcm;

		int samples = vorbis_synthesis_pcmout(&vd, &pcm);
		if (samples <= 0)
		{
			stream_data();
			continue;
		}
		
		if (samples > data_left) samples = data_left;

		int buffer_pos = data_requested-data_left;
		short **buffers = (short **) data_ptr;
		for (int j=0; j<vi.channels; j++)
		{
			for (int i=0; i<samples; i++)
			{
				int s = (int) (pcm[j][i] * 32767);
				if (s < -32767) s = -32767;
				else if (s > 32767) s = 32767;
				buffers[j][i+buffer_pos] = (short) s;
			}
		}

		vorbis_synthesis_read(&vd, samples);
		data_left -= samples;

		position += samples;
		if (num_samples < position) num_samples = position;
	}
	
	return data_requested - data_left;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProvider_Vorbis_Session implementation:

void CL_SoundProvider_Vorbis_Session::stream_data()
{
	while (!eof())
	{
		int result=ogg_stream_packetout(&os,&op);
		if (result > 0)
		{
			if(vorbis_synthesis(&vb,&op)==0) /* test for success! */
				vorbis_synthesis_blockin(&vd,&vb);
			break;
		}
		
		while (!eof())
		{
			int result = ogg_sync_pageout(&oy,&og);
			if (result == -1) continue; // corrupt data at this page position. Read next.
			if (result == 0) // need more data
			{
				char *buffer=ogg_sync_buffer(&oy,4096);
				int bytes=input->read(buffer,4096);
				ogg_sync_wrote(&oy,bytes);
				if (bytes==0)
				{
					stream_eof = true;
					return;
				}
			}
			else
			{
				ogg_stream_pagein(&os,&og);
				break;
			}
		}
	}
}
