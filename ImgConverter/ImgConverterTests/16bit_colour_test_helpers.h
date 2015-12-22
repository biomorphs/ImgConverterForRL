#ifndef R5G6B5_COLOUR_TEST_HELPERS_INCLUDED
#define R5G6B5_COLOUR_TEST_HELPERS_INCLUDED

// Helper for building 16bit colours
#define MAKE_R5G6B5(r,g,b)	(((uint16_t)r >> 3) << 11) | (((uint16_t)g >> 2) << 5) | ((uint16_t)b >> 3)

// Helper for testing quantised values with error
#define APPROX_MATCH_QUANTISED(maxError, source, target)	\
			( (int32_t)source >= ((int32_t)target-(int32_t)maxError) && (int32_t)source <= ((int32_t)target+(int32_t)maxError) )

#define APPROX_MATCH_COLOUR(source, target)	\
	( APPROX_MATCH_QUANTISED(8, source.GetRed(), target.GetRed()) && \
	APPROX_MATCH_QUANTISED(4, source.GetGreen(), target.GetGreen()) && \
	APPROX_MATCH_QUANTISED(8, source.GetBlue(), target.GetBlue()) )

#endif