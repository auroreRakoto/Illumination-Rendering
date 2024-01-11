#version 330 core

in VertexData
{
    vec3 position_view;
    vec3 normal_view;
    vec3 tangent_view;
    vec3 bitangent_view;
    vec2 texCoord;
    vec3 light_view;
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

out vec4 color;

//uniform sampler2D myTextureSampler0;
//uniform sampler2D myTextureSampler1;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;


uniform int switch_text;
uniform int parallax;
// n is the normal (should be normalized), 
// light_dir the direction TO the light (should be normalized),
// cam_dir the direction TO the camera (should be normalized),
// color_mat the color of the object for the current fragment,
// color_light the color of the light 
vec3 phong(vec3 n, vec3 light_dir, vec3 cam_dir, vec3 color_mat, vec3 color_light, float ka, float kd, float ks, int shininess)
{
  n = normalize(n);  // IS IT NEEDED?

  float ambiant = ka;
  float diffuse = kd*clamp(dot(n, light_dir), 0.0, 1.0);
  float specular = ks*pow(clamp(dot(cam_dir, reflect(-light_dir, n)), 0.0, 1.0), shininess);

  return (ambiant + diffuse) * color_mat + specular * color_light;
}

vec3 phong(vec3 n, vec3 light_dir1, vec3 light_dir2, vec3 cam_dir, vec3 color_mat, vec3 color_light1, vec3 color_light2, float ka, float kd, float ks, int shininess)
{
    n = normalize(n);

    // Contribution de la première source de lumière
    float ambiant1 = ka;
    float diffuse1 = kd * clamp(dot(n, light_dir1), 0.0, 1.0);
    float specular1 = ks * pow(clamp(dot(cam_dir, reflect(-light_dir1, n)), 0.0, 1.0), shininess);
    vec3 light_contribution1 = ((ambiant1 + diffuse1) * color_mat + specular1) * color_light1;

    // Contribution de la deuxième source de lumière
    float ambiant2 = ka;
    float diffuse2 = kd * clamp(dot(n, light_dir2), 0.0, 1.0);
    float specular2 = ks * pow(clamp(dot(cam_dir, reflect(-light_dir2, n)), 0.0, 1.0), shininess);
    vec3 light_contribution2 = ((ambiant2 + diffuse2) * color_mat + specular2) * color_light2;

    // Addition des contributions des deux sources lumineuses
    return light_contribution1 + light_contribution2;
}


vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    float height = (texture(texture0, texCoords).xyz).r;  
    vec2 p = viewDir.xy / viewDir.z * height;
    return texCoords - p;  
    //return vec2(viewDir.xy) ;    
} 

vec2 ParallaxMapping2(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * 0.2; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(texture2, currentTexCoords).r;
      
    int maxIterations = 100; // Ajout d'une limite d'itérations
    
    for (int i = 0; i < maxIterations && currentLayerDepth < currentDepthMapValue; ++i)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(texture2, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(texture2, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}
void main()
{

  vec3 viewDir = vsData.cam_tan;
  //vec3 viewDir = normalize(-vsData.position_tan);
  //vec3 viewDir = normalize(vsData.view_pos_tan - vsData.TangentFragPos); 
  vec2 texCoords;
  if (parallax == 0)
  {
    texCoords = vsData.texCoord;
  } else 
  {
      texCoords = ParallaxMapping(vsData.texCoord,  viewDir);
      if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
      {
        //discard;
      }
  }
  
    
  
    
    //texCoords = vsData.texCoord;

  float ka = 0.2 * (texture(texture1, texCoords).xyz).r;
  float kd = 0.8;
  float ks = 0.3;
  int shininess = 256;
  
  //if (switch_text == 0)
  //{
  //  color_mat = texture(texture0, vsData.texCoord).xyz ;
  //}
  //if (switch_text == 1)
  //{
  //  color_mat = texture(texture1, vsData.texCoord).xyz ;
  //}
  //if (switch_text == 2)
  //{
  //  color_mat = texture(texture2, vsData.texCoord).xyz ;
  //}
  //if (switch_text == 3)
  //{
  //  color_mat = texture(texture3, vsData.texCoord).xyz ;
  //}
  //if (switch_text == 4)
  //{
  //  color_mat = texture(texture4, vsData.texCoord).xyz ;
  //}

  

  vec3 normal_tan = 2.0f*texture(texture3, texCoords).xyz - 1.0f;


  vec3 color_mat = texture(texture0, texCoords).xyz ;

  //color = vec4(color_mat,1.0);


    // Use everything in camera space
    //color = vec4(phong(normalize(normal_tan), normalize(vsData.light_tan), vsData.cam_tan, color_mat, vec3(1.0, 1.0, 1.0), ka, kd, ks, shininess), 1.0);
  //color = vec4(phong(normalize(vsData.normal_view), normalize(vsData.light_view), -normalize(vsData.position_view),
   //color_mat, vec3(1.0, 1.0, 1.0), ka, kd, ks, shininess), 1.0);

   //multiple light
   color = vec4(phong(normalize(normal_tan), normalize(vsData.light_tan),normalize(vsData.light_tan2), vsData.cam_tan_fix, color_mat, vec3(1.0, 0.0, 0.0),vec3(0.0, 0.0, 1.0), ka, kd, ks, shininess), 1.0);
   //color = vec4(normalize(texture(texture3, texCoords).xyz) , 1.0);
}