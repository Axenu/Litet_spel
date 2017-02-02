// #version 400 core
//
// //uniform sampler2D _texture;
//
// //in vec2 pass_UV;
//
// out vec4 Frag_Data;
//
// void main() {
//     //Frag_Data = vec4(1) * texture(_texture, pass_UV).r;
//     Frag_Data = vec4(0,1, 1, 1);
// }

#version 400 core

out vec4 Frag_Data;

in vec2 pass_UV;

uniform sampler2D _texture;

void main() {
    // Frag_Data = vec4(pass_UV,1,1);
    Frag_Data = texture(_texture, pass_UV);
}
