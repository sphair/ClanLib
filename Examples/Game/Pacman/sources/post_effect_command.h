
#pragma once

class PostEffectCommand : public CL_PixelCommand
{
public:
	PostEffectCommand(int transparency) : transparency(transparency) { }
	void run(CL_PixelThreadContext *context);

private:
	int transparency;
};

inline void PostEffectCommand::run(CL_PixelThreadContext *context)
{
	// int lum = c.red*0.3 + c.green*0.59 + c.blue*0.11;

	int mult_r = 76*transparency/256;
	int mult_g = 150*transparency/256;
	int mult_b = 28*transparency/256;

	__m128i const0, const1, one, half;
	CL_BlitARGB8SSE::set_one(one);
	CL_BlitARGB8SSE::set_half(half);
	const0 = _mm_set_epi16(0, mult_r, mult_g, mult_b, 0, mult_r, mult_g, mult_b);
	const1 = _mm_set_epi16(transparency, 0, 0, 0, transparency, 0, 0, 0);

	int sse_width = (context->colorbuffer0.size.width/2)*2;
	for (int y = context->core; y < context->colorbuffer0.size.height; y += context->num_cores)
	{
		unsigned int *line = context->colorbuffer0.data + context->colorbuffer0.size.width * y;
		for (int x = 0; x < sse_width; x+=2)
		{
			__m128i dest0, bw0, tmp;
			CL_BlitARGB8SSE::load_pixels(dest0, line+x);

			bw0 = _mm_srli_epi16(_mm_mullo_epi16(dest0, const0), 8);
			tmp = _mm_shufflehi_epi16(bw0, _MM_SHUFFLE(3, 3, 0, 1));
			tmp = _mm_shufflelo_epi16(tmp, _MM_SHUFFLE(3, 3, 0, 1));
			bw0 = _mm_add_epi16(bw0, tmp);
			tmp = _mm_shufflehi_epi16(bw0, _MM_SHUFFLE(3, 3, 3, 2));
			tmp = _mm_shufflelo_epi16(tmp, _MM_SHUFFLE(3, 3, 3, 2));
			bw0 = _mm_add_epi16(bw0, tmp);
			bw0 = _mm_shufflehi_epi16(bw0, _MM_SHUFFLE(3, 0, 0, 0));
			bw0 = _mm_shufflelo_epi16(bw0, _MM_SHUFFLE(3, 0, 0, 0));
			bw0 = _mm_add_epi16(bw0, const1);

			CL_BlitARGB8SSE::blend_premultiplied(dest0, bw0, one, half);
			CL_BlitARGB8SSE::store_pixels(line+x, dest0);
		}
	}
}
