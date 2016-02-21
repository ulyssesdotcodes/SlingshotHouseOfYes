in vec4 ciPosition;
out int vInstance;

void main()
{
    gl_Position = ciPosition;
    vInstance = gl_InstanceID;
}