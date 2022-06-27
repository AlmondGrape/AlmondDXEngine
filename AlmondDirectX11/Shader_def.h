#pragma once

enum class SHADER_TYPE
{
	VERTEX_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER
};

enum class CBV_REGISTER : unsigned __int32
{
	b0 = 0,
	b1 = 1,
	b2 = 2,
	b3 = 3,
	b4 = 4,
	END
};

enum class SRV_REGISTER : unsigned __int32
{
	t0 = 0,
	t1 = 1,
	t2 = 2,
	t3 = 3,
	t4 = 4,
	END
};

enum class UAV_REGISTER : unsigned __int32
{
	u0 = 0,
	u1 = 1,
	u2 = 2,
	u3 = 3,
	u4 = 4,
	END
};

enum class SAM_REGISTER : unsigned __int32
{
	s0 = 0,
	s1 = 1,
	s2 = 2,
	s3 = 3,
	s4 = 4,
	END
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
};