// hello world fragment shader
#define PI 3.14159265359
#define MAX_MARCH 99
#define MARCH_INC 0.1

struct Material {
  float ka;
  float kd;
  float ks;
  float s;

  vec3 color;
};

struct Light {
  vec3 pos;
  vec3 diffuse_color;
  vec3 specular_color;
};

vec3 GetIllumination(vec3 cam_pos, vec3 p, Light l, Material m) {
  vec3 dir_to_light = normalize(l.pos - p);
  vec3 surface_normal = vec3(0.0, 1.0, 0.0);

  vec3 dir_to_viewer = normalize(vec3(cam_pos - p));
  vec3 reflection_dir = normalize(2.0 * dot(dir_to_light, surface_normal) * surface_normal - dir_to_light);

  vec3 ambient_i = m.ka * vec3(0.2);
  vec3 diffuse_i = m.kd * dot(dir_to_light, surface_normal) * l.diffuse_color;
  vec3 specular_i = m.ks * pow(dot(reflection_dir, dir_to_viewer), m.s) * l.specular_color;

  return ambient_i + diffuse_i + specular_i;
}

float GetDist(vec3 p, out Material m) {

  // y = 0 plane
  float floorDist = p.y;
  m = Material(0.3, 0.5, 0.5, 9.0, vec3(0.0, 0.4, 0.1));

  return floorDist;
}
void main(void)
{
  // Get uv 0 to 1 and clip space -1 to 1 coords
  vec2 uv = vec2(gl_FragCoord.x/ 1280.0, gl_FragCoord.y / 1020.0);
	vec2 cp = vec2(uv.x*2.0 - 1.0, uv.y*2.0 - 1.0);

  // Define the light
  Light light = Light(vec3(0.0, 5.0, 19.0), vec3(0.0, 1.0, 0.0), vec3(1.0, 1.0, 1.0));
  // Light light = Light(vec3(0.0, 2.0, 9.0), vec3(0.0), vec3(0.0));

  // Define the camera
  vec3 world_up = vec3(0.0,1.0,0.0);
  vec3 cam_pos = vec3(0.0,1.0,0.0);
  vec3 cam_dir = normalize(vec3(0.0,0.0,1.0));
  vec3 cam_right = normalize(cross(world_up, cam_dir));
  vec3 cam_up = normalize(cross(cam_dir, cam_right));
  float fov = 45.0;

  // Define the ray to march along
  vec3 ray_origin = cam_pos;
  float up_diff = cp.y * sin(((fov/2.0) / 360.0) * 2.0 * PI);
  float right_diff = cp.x * cos(((fov/2.0) / 360.0) * 2.0 * PI);
  vec3 ray_dir = normalize(cam_dir + up_diff*cam_up + right_diff*cam_right);

  // Ray march
  vec3 p = ray_origin;
  float last_dist = 0.0;
  Material nearest_material = Material(0.0, 0.0, 0.0, 0.0, vec3(0.0));
  for (int i = 0; i < MAX_MARCH; i++) {
    p = p + ray_dir * last_dist;
    float dist = GetDist(p, nearest_material);
    if (dist <= 0.001) {
      vec3 illumination = GetIllumination(cam_pos, p, light, nearest_material);
      gl_FragColor = vec4(illumination,1.0);
      return;
    }
    last_dist = dist;
  }

	gl_FragColor = vec4(0.0,0.0,0.2, 1.0);
}
