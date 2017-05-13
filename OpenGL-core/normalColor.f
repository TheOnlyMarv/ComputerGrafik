#version 330
smooth in vec4 myColor;
out vec4 outColor;
void main(){
   //outColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	outColor = vec4((myColor.x +1)/2 , (myColor.y +1)/2, (myColor.z +1)/2, 1.0f);
}