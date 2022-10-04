#version 330 core

in vec4 gl_FragCoord;
out vec4 out_col;

uniform vec3 in_col1, in_col2;
uniform int max_it, width, height;

#define dist(a) a.x * a.x + a.y * a.y
#define add(a, b) vec2(a.x + b.x, a.y + b.y)
#define sub(a, b) vec2(a.x - b.x, a.y - b.y)
#define mul(a, b) vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x)
#define div(a, b) vec2(((a.x * b.x + a.y * b.y)/(b.x * b.x + b.y * b.y)),((a.y * b.x - a.x * b.y)/(b.x * b.x + b.y * b.y)))



int pos_iterations()
{
	float real_pos = (2.0f * float(gl_FragCoord.x) / float(width)) - delta_x;
	float imag_pos = (2.0f * float(gl_FragCoord.y) / float(height)) - delta_y;

	int it = 0;
	vec2 cur_pos = vec2(0.0, 0.0);
	vec2 const_pos = vec2(real_pos, imag_pos);
	vec2 one = vec2(1.0, 0.0);
	vec2 two = vec2(2.0, 0.0);

	while (dist(cur_pos) < 4.0 && it < max_it)
	{
		vec2 numerator = add(mul(cur_pos, cur_pos), sub(const_pos, one));
		vec2 denominator = add(mul(two, cur_pos), sub(const_pos, two));
		vec2 fraction = div(numerator, denominator);
		cur_pos = mul(fraction, fraction);
		it++;
	}

	return it;
}

vec3 pos_color()
{
	int it = pos_iterations();
	if (it == max_it)
	{
		return vec3(0.0f, 0.0f, 0.0f);
	}

	float alpha = float(it) / max_it;
	return mix(in_col1, in_col2, alpha);
}

void main()
{
	vec3 frag_col = pos_color();
	out_col = vec4(frag_col.rgb, 1.0f);
}

if (gl_FragCoord.x < 320.0f || gl_FragCoord.y > 640.0f)
	{
        out_col = vec4(in_col1.rgb, 1.0f);
	} else {
		vec3 frag_col = pos_color();
		out_col = vec4(frag_col.rgb, 1.0f);
	}