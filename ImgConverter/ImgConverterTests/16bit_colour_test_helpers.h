#ifndef R5G6B5_COLOUR_TEST_HELPERS_INCLUDED
#define R5G6B5_COLOUR_TEST_HELPERS_INCLUDED

// Helper for building 16bit colours
#define MAKE_R5G6B5(r,g,b)	(((uint16_t)r >> 3) << 11) | (((uint16_t)g >> 2) << 5) | ((uint16_t)b >> 3)

// Helper for testing quantised values with error
#define APPROX_MATCH_QUANTISED(maxError, source, target)	\
			( source >= (target-maxError) && source <= (target+maxError) )

#endif