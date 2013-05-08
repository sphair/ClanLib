lib_LTLIBRARIES = libclan30Scene3D.la

libclan30Scene3D_la_SOURCES = \
Performance/gpu_timer.cpp \
Performance/scope_timer.cpp \
Level/level.cpp \
Passes/LogAverageLight/log_average_light.cpp \
Passes/ParticleEmitter/particle_emitter_pass.cpp \
Passes/ParticleEmitter/particle_emitter_pass_data.cpp \
Passes/VSMShadowMap/vsm_shadow_map_pass.cpp \
Passes/ZMinMax/z_minmax.cpp \
Passes/Transparency/transparency_pass.cpp \
Passes/Lightsource/lightsource_pass.cpp \
Passes/LightsourceSimple/lightsource_simple_pass.cpp \
Passes/Skybox/skybox_pass.cpp \
Passes/Final/final_pass.cpp \
Passes/SSAO/ssao_pass.cpp \
Passes/GaussianBlur/gaussian_blur.cpp \
Passes/Bloom/bloom_pass.cpp \
Passes/DiffuseGI/diffuse_gi_pass_cs.cpp \
Passes/GBuffer/gbuffer_pass.cpp \
scene_camera.cpp \
scene_cache.cpp \
scene_pass.cpp \
precomp.cpp \
Culling/intersection_test.cpp \
Culling/PortalMap/portal_map.cpp \
Culling/PortalMap/portal_clipping.cpp \
Culling/QuadTree/quad_tree.cpp \
Culling/QuadTree/quad_tree_node.cpp \
Culling/OctTree/oct_tree.cpp \
Culling/OctTree/oct_tree_node.cpp \
Culling/clipping_frustum.cpp \
scene_particle_emitter.cpp \
scene_object.cpp \
scene.cpp \
Model/model_shader_cache.cpp \
Model/model.cpp \
Model/model_instance.cpp \
Model/model_lod.cpp \
Model/model_cache.cpp \
Framework/instances_buffer.cpp \
Framework/material_cache.cpp \
Framework/shader_setup.cpp \
Framework/shadow_maps.cpp \
scene_model.cpp \
scene_light.cpp

libclan30Scene3D_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanScene3D)

libclan30Scene3D_la_CXXFLAGS=$(clanScene3D_CXXFLAGS) $(extra_CFLAGS_clanScene3D)
# EOF #

