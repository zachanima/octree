#include "heightmap.hpp"

GLfloat Heightmap::simplex(glm::vec3 v, GLuint octaves, GLfloat persistence, GLfloat scale) {
  GLfloat total = 0.f;
  GLfloat frequency = scale;
  GLfloat amplitude = 1.f;

  GLfloat maxAmplitude = 0.f;

  for (size_t i = 0; i < octaves; i++) {
    total += ridge(snoise(v * frequency), 1.f) * amplitude;
    frequency *= 2.f;
    maxAmplitude += amplitude;
    amplitude *= persistence;
  }

  return total / maxAmplitude;
}



glm::vec3 Heightmap::mod289(glm::vec3 x) {
  return x - glm::floor(x * (1.f / 289.f)) * 289.f;
}



glm::vec4 Heightmap::mod289(glm::vec4 x) {
  return x - glm::floor(x * (1.f / 289.f)) * 289.f;
}



glm::vec4 Heightmap::permute(glm::vec4 x) {
  return mod289((x * 34.f + 1.f) * x);
}



glm::vec4 Heightmap::taylorInvSqrt(glm::vec4 r) {
  return 1.79284291400159f - 0.85373472095314f * r;
}



GLfloat Heightmap::snoise(glm::vec3 v) {
  const glm::vec2 C = glm::vec2(1.f / 6.f, 1.f / 3.f);
  const glm::vec4 D = glm::vec4(0.f, 0.5f, 1.f, 2.f);

  // First corner.
  glm::vec3 i = glm::floor(v + glm::dot(v, glm::vec3(C.y, C.y, C.y)));
  glm::vec3 x0 = v - i + glm::dot(i, glm::vec3(C.x, C.x, C.x));

  // Other corners.
  glm::vec3 g = glm::step(glm::vec3(x0.y, x0.z, x0.x), x0);
  glm::vec3 l = 1.f - g;
  glm::vec3 i1 = glm::min(g, glm::vec3(l.z, l.x, l.y));
  glm::vec3 i2 = glm::max(g, glm::vec3(l.z, l.x, l.y));

  glm::vec3 x1 = x0 - i1 + glm::vec3(C.x, C.x, C.x);
  glm::vec3 x2 = x0 - i2 + glm::vec3(C.y, C.y, C.y);
  glm::vec3 x3 = x0 - glm::vec3(D.y, D.y, D.y);

  // Permutations.
  i = mod289(i);
  glm::vec4 p = permute(permute(permute(
    i.z + glm::vec4(0.f, i1.z, i2.z, 1.f)) +
    i.y + glm::vec4(0.f, i1.y, i2.y, 1.f)) +
    i.x + glm::vec4(0.f, i1.x, i2.x, 1.f)
  );

  // Gradients.
  GLfloat n_ = 0.142857142857f;
  glm::vec3 ns = n_ * glm::vec3(D.w, D.y, D.z) - glm::vec3(D.x, D.z, D.x);

  glm::vec4 j = p - 49.f * glm::floor(p * ns.z * ns.z);

  glm::vec4 x_ = glm::floor(j * ns.z);
  glm::vec4 y_ = glm::floor(j - 7.f * x_);

  glm::vec4 x = x_ * ns.x + glm::vec4(ns.y, ns.y, ns.y, ns.y);
  glm::vec4 y = y_ * ns.x + glm::vec4(ns.y, ns.y, ns.y, ns.y);
  glm::vec4 h = 1.f - glm::abs(x) - glm::abs(y);

  glm::vec4 b0 = glm::vec4(x.x, x.y, y.x, y.y);
  glm::vec4 b1 = glm::vec4(x.z, x.w, y.z, y.w);

  glm::vec4 s0 = glm::floor(b0) * 2.f + 1.f;
  glm::vec4 s1 = glm::floor(b1) * 2.f + 1.f;
  glm::vec4 sh = -glm::step(h, glm::vec4(0.f));

  glm::vec4 a0 = glm::vec4(b0.x, b0.z, b0.y, b0.w) + glm::vec4(s0.x, s0.z, s0.y, s0.w) * glm::vec4(sh.x, sh.x, sh.y, sh.y);
  glm::vec4 a1 = glm::vec4(b1.x, b1.z, b1.y, b1.w) + glm::vec4(s1.x, s1.z, s1.y, s1.w) * glm::vec4(sh.z, sh.z, sh.w, sh.w);

  glm::vec3 p0 = glm::vec3(a0.x, a0.y, h.x);
  glm::vec3 p1 = glm::vec3(a0.z, a0.w, h.y);
  glm::vec3 p2 = glm::vec3(a1.x, a1.y, h.z);
  glm::vec3 p3 = glm::vec3(a1.z, a1.w, h.w);

  // Normalize gradients.
  glm::vec4 norm = taylorInvSqrt(glm::vec4(glm::dot(p0, p0), glm::dot(p1, p1), glm::dot(p2, p2), glm::dot(p3, p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

  // Mix final noise value.
  glm::vec4 m = glm::max(0.6f - glm::vec4(glm::dot(x0, x0), glm::dot(x1, x1), glm::dot(x2, x2), glm::dot(x3, x3)), 0.f);
  m = m * m;

  return 42.f * glm::dot(m * m, glm::vec4(
    glm::dot(p0, x0), glm::dot(p1, x1), glm::dot(p2, x2), glm::dot(p3, x3)
  ));
}



GLfloat Heightmap::ridge(GLfloat h, GLfloat offset) {
  h = glm::abs(h);
  h = offset - h;
  h = h * h;
  return h;
}
