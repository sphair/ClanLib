
#pragma once

class RTPPacketAVP
{
public:
	// payload types 96-127 = dynamic
	// marker bit = sound started against after silence
	// packet based encodings: default to 20 ms/packet
	// frame based encodings: may put several frames in same packet
	// Minimum audio support: payload types 0 (PCMU) and 5 (DVI4)
	// Default ports: 5004 and 5005
};



// RTP payload
// RTP packet
// RTP media type
// RTP session
// Multimedia session (the combined session, i.e. audio rtp session + video rtp session)
// RTCP packet

// Synchronization source (SSRC) = random number as generated in Appendix A.6
// Contributing source (CSRC)

// network byte order
// NTP timestamp = seconds since 1 January 1900

// Profiles are typically titled "RTP Profile for ..."
// Payload data, such as H.261 encoded video, are typically titled "RTP Payload Format for XYZ Audio/Video Encoding"

class RTPFixedHeader
{
public:
	unsigned int version:2;
	unsigned int padding:1;
	unsigned int extension:1;
	unsigned int csrc_count:4;
	unsigned int marker:1;
	unsigned int payload_type:7;
	unsigned int sequence_number:16;
	unsigned int timestamp;
	unsigned int synchronization_source; // ssrc
	std::vector<unsigned int> contributing_sources; // csrc (only present when coming from a mixer)

	// Only if extension bit is set:
	unsigned int profile_specific:16;
	unsigned int extension_length:16:
	CL_DataBuffer extension; // extension_length in size
};

/*
   12.1 RTCP Packet Types

   abbrev.  name                 value
   SR       sender report          200
   RR       receiver report        201
   SDES     source description     202
   BYE      goodbye                203
   APP      application-defined    204

   12.2 SDES Types

   abbrev.  name                            value
   END      end of SDES list                    0
   CNAME    canonical name                      1
   NAME     user name                           2
   EMAIL    user's electronic mail address      3
   PHONE    user's phone number                 4
   LOC      geographic user location            5
   TOOL     name of application or tool         6
   NOTE     notice about the source             7
   PRIV     private extensions                  8
*/


#ifdef from_rfc3550

/*
* rtp.h  --  RTP header file
*/
#include <sys/types.h>

/*
* The type definitions below are valid for 32-bit architectures and
* may have to be adjusted for 16- or 64-bit architectures.
*/
typedef unsigned char  u_int8;
typedef unsigned short u_int16;
typedef unsigned int   u_int32;
typedef          short int16;

/*
* Current protocol version.
*/
#define RTP_VERSION    2

#define RTP_SEQ_MOD (1<<16)
#define RTP_MAX_SDES 255      /* maximum text length for SDES */

typedef enum
{
   RTCP_SR   = 200,
   RTCP_RR   = 201,
   RTCP_SDES = 202,
   RTCP_BYE  = 203,
   RTCP_APP  = 204
} rtcp_type_t;

typedef enum
{
   RTCP_SDES_END   = 0,
   RTCP_SDES_CNAME = 1,
   RTCP_SDES_NAME  = 2,
   RTCP_SDES_EMAIL = 3,
   RTCP_SDES_PHONE = 4,
   RTCP_SDES_LOC   = 5,
   RTCP_SDES_TOOL  = 6,
   RTCP_SDES_NOTE  = 7,
   RTCP_SDES_PRIV  = 8
} rtcp_sdes_type_t;

/*
* RTP data header
*/
typedef struct
{
   unsigned int version:2;   /* protocol version */
   unsigned int p:1;         /* padding flag */
   unsigned int x:1;         /* header extension flag */
   unsigned int cc:4;        /* CSRC count */
   unsigned int m:1;         /* marker bit */
   unsigned int pt:7;        /* payload type */
   unsigned int seq:16;      /* sequence number */
   u_int32 ts;               /* timestamp */
   u_int32 ssrc;             /* synchronization source */
   u_int32 csrc[1];          /* optional CSRC list */
} rtp_hdr_t;

/*
* RTCP common header word
*/
typedef struct
{
   unsigned int version:2;   /* protocol version */
   unsigned int p:1;         /* padding flag */
   unsigned int count:5;     /* varies by packet type */
   unsigned int pt:8;        /* RTCP packet type */
   u_int16 length;           /* pkt len in words, w/o this word */
} rtcp_common_t;

/*
* Big-endian mask for version, padding bit and packet type pair
*/
#define RTCP_VALID_MASK (0xc000 | 0x2000 | 0xfe)
#define RTCP_VALID_VALUE ((RTP_VERSION << 14) | RTCP_SR)

/*
* Reception report block
*/
typedef struct
{
   u_int32 ssrc;             /* data source being reported */
   unsigned int fraction:8;  /* fraction lost since last SR/RR */
   int lost:24;              /* cumul. no. pkts lost (signed!) */
   u_int32 last_seq;         /* extended last seq. no. received */
   u_int32 jitter;           /* interarrival jitter */
   u_int32 lsr;              /* last SR packet from this source */
   u_int32 dlsr;             /* delay since last SR packet */
} rtcp_rr_t;

/*
* SDES item
*/
typedef struct
{
   u_int8 type;              /* type of item (rtcp_sdes_type_t) */
   u_int8 length;            /* length of item (in octets) */
   char data[1];             /* text, not null-terminated */
} rtcp_sdes_item_t;

/*
* One RTCP packet
*/
typedef struct
{
   rtcp_common_t common;     /* common header */
   union
   {
       /* sender report (SR) */
       struct
	   {
           u_int32 ssrc;     /* sender generating this report */
           u_int32 ntp_sec;  /* NTP timestamp */
           u_int32 ntp_frac;
           u_int32 rtp_ts;   /* RTP timestamp */
           u_int32 psent;    /* packets sent */
           u_int32 osent;    /* octets sent */
           rtcp_rr_t rr[1];  /* variable-length list */
       } sr;

       /* reception report (RR) */
       struct
	   {
           u_int32 ssrc;     /* receiver generating this report */
           rtcp_rr_t rr[1];  /* variable-length list */
       } rr;

       /* source description (SDES) */
       struct rtcp_sdes
	   {
           u_int32 src;      /* first SSRC/CSRC */
           rtcp_sdes_item_t item[1]; /* list of SDES items */
       } sdes;

       /* BYE */
       struct
	   {
           u_int32 src[1];   /* list of sources */
           /* can't express trailing text for reason */
       } bye;
   } r;
} rtcp_t;

typedef struct rtcp_sdes rtcp_sdes_t;

/*
* Per-source state information
*/
typedef struct
{
   u_int16 max_seq;        /* highest seq. number seen */
   u_int32 cycles;         /* shifted count of seq. number cycles */
   u_int32 base_seq;       /* base seq number */
   u_int32 bad_seq;        /* last 'bad' seq number + 1 */
   u_int32 probation;      /* sequ. packets till source is valid */
   u_int32 received;       /* packets received */
   u_int32 expected_prior; /* packet expected at last interval */
   u_int32 received_prior; /* packet received at last interval */
   u_int32 transit;        /* relative trans time for prev pkt */
   u_int32 jitter;         /* estimated jitter */
   /* ... */
} source;

#endif
