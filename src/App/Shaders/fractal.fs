#version 330 core

in vec4 gl_FragCoord;
out vec4 out_col;

uniform vec3 in_col1, in_col2;
uniform int max_it;
uniform mat4 transform;

#define dist(a) a.x * a.x + a.y * a.y
#define add(a, b) vec2(a.x + b.x, a.y + b.y)
#define sub(a, b) vec2(a.x - b.x, a.y - b.y)
#define mul(a, b) vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x)
#define div(a, b) vec2(((a.x * b.x + a.y * b.y)/(b.x * b.x + b.y * b.y)),((a.y * b.x - a.x * b.y)/(b.x * b.x + b.y * b.y)))

#define CONST_SIZE 640.0f

int pos_iterations()
{
	vec4 point = vec4(gl_FragCoord.x / CONST_SIZE, gl_FragCoord.y / CONST_SIZE, 0.0f, 1.0f);
	point = transform * point;

	int it = 0;
	vec2 cur_pos = vec2(0.0, 0.0);
	vec2 const_pos = vec2(point.xy);
	vec2 one = vec2(1.0, 0.0);
	vec2 two = vec2(2.0, 0.0);

	while (dist(cur_pos) < 10.0 && it < max_it)
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

	float alpha = pow(float(it) / float(max_it), 0.2);
	return mix(in_col1, in_col2, alpha);
}

void main()
{	
	vec3 frag_col = pos_color();
	out_col = vec4(frag_col.rgb, 1.0f);
}