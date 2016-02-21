
uniform mat4 ciModelViewProjection;
uniform mat4 ciProjectionMatrix;

in vec4 ciPosition;
in vec2 ciTexCoord0;

out vec2 vertTexCoord0;
out vec4 vPosition;

void main()
{
	// Simply pass the texture coordinate and the position.
	vertTexCoord0 = ciTexCoord0;
	gl_Position = ciModelViewProjection * ciPosition;
	vPosition = ciPosition;
}
