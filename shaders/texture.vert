#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec2 texcoord;


out VertexData
{
    vec3 position_view; //position in camera space
    vec3 normal_view; //normal in camera space
    vec3 tangent_view; //tangent in camera space
    vec3 bitangent_view; //bitangent in camera space
    vec2 texCoord; //texture coordinate
    vec3 light_view; // light direction in camera space
    vec3 light_view2;

    vec3 position_tan;
    vec3 light_tan;
    vec3 light_tan2;

    vec3 cam_tan;

    vec3 cam_tan_fix;

    vec3 view_pos_tan;

    vec3 TangentFragPos;
    vec3 FragPos;

    vec3 light_fix;
    vec3 light_fix2;
} vsData;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Perspective;


uniform sampler2D texture2;

uniform vec3 view_pos;

vec3 light_pos = vec3(10.0, 20.0, 0.0);
vec3 light_pos2 = vec3(-10.0, 20.0, 0.0);

void main()
{
    float hauteur = 0.2 * (texture(texture2, texcoord).xyz).r;
    vec3 posHauteur = position + hauteur * normal;
    gl_Position = Perspective * View * Model * vec4(posHauteur, 1.0);
    
    //gl_Position = Perspective * View * Model * vec4(position, 1.0);


    // Send all information in camera space
    vsData.position_view = (View * Model * vec4(posHauteur, 1.0)).xyz;
    //vsData.position_view = (View * Model * vec4(position, 1.0)).xyz;
    // For explanation https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
    vsData.normal_view = (transpose(inverse(View * Model)) * vec4(normal, 0.0)).xyz;
    vsData.tangent_view = normalize((View * Model * vec4(tangent.xyz, 0.0)).xyz);
    vsData.bitangent_view = normalize(cross(vsData.tangent_view, vsData.normal_view)*tangent.w);

    

    vsData.texCoord = texcoord;

    //lumière une
    vec3 light_pos_view = (View * vec4(light_pos, 1.0)).xyz;
    vsData.light_view = normalize(light_pos_view - vsData.position_view);

    //lumière 2
    vec3 light_pos_view2 = (View * vec4(light_pos2, 1.0)).xyz;
    vsData.light_view2 = normalize(light_pos_view2 - vsData.position_view);


    vec3 b = normalize(cross(vsData.tangent_view, vsData.normal_view)*tangent.w);
    // Matrice de passage du repere view au repere tangant
    mat3 TBN = transpose(mat3(vsData.tangent_view, b, vsData.normal_view ));

    vsData.position_tan = TBN * vsData.position_view; 
    //vsData.light_tan = TBN * (normalize(vsData.light_view-vsData.position_view));
    //vsData.light_tan2 = TBN * (normalize(vsData.light_view2-vsData.position_view));
    vsData.light_tan = TBN * (normalize(vsData.light_view));
    vsData.light_tan2 = TBN * (normalize(vsData.light_view2));


    vsData.cam_tan = TBN * (-normalize(vsData.position_view)); 


    
    vec3 T = normalize(mat3(Model) * tangent.xyz);
    
    vec3 N = normalize(mat3(Model) * vec3(normal)).xyz;
    vec3 B = normalize(cross(T, N)*tangent.w);

    mat3 TBN_fix = transpose(mat3(T, B, N));
    vsData.cam_tan_fix = TBN_fix * (-normalize(Model*vec4(position,1.0f)).xyz);
    
    vsData.view_pos_tan  = TBN_fix * view_pos;
    vsData.FragPos   = vec3( Model * vec4(position, 1.0));
    vsData.TangentFragPos  = TBN_fix * vsData.FragPos;

    //Changement de base pour la visualisation des deux lumières 
    vsData.light_fix = normalize(TBN_fix * (normalize(Model*vec4(light_pos,1.0f)).xyz)); 
    vsData.light_fix2 = normalize(TBN_fix * (normalize(Model*vec4(light_pos2,1.0f)).xyz)); 
}