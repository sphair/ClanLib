lib_LTLIBRARIES = libclan30Scene3D.la

libclan30Scene3D_la_SOURCES = \
scene.cpp \
scene_light_probe.cpp \
precomp.cpp \
scene_particle_emitter.cpp \
scene_model.cpp \
Passes/VSMShadowMap/vsm_shadow_map_pass.cpp \
Passes/LightsourceSimple/lightsource_simple_pass.cpp \
Passes/Bloom/bloom_pass.cpp \
Passes/ZMinMax/z_minmax.cpp \
Passes/ParticleEmitter/particle_emitter_pass_data.cpp \
Passes/ParticleEmitter/particle_emitter_pass.cpp \
Passes/LogAverageLight/log_average_light.cpp \
Passes/DiffuseGI/diffuse_gi_pass_cs.cpp \
Passes/GBuffer/gbuffer_pass.cpp \
Passes/Lightsource/lightsource_pass.cpp \
Passes/Skybox/skybox_pass.cpp \
Passes/GaussianBlur/gaussian_blur.cpp \
Passes/SSAO/ssao_pass.cpp \
Passes/Final/final_pass.cpp \
Passes/Transparency/transparency_pass.cpp \
Culling/OctTree/oct_tree_node.cpp \
Culling/OctTree/oct_tree.cpp \
Culling/RTree/rtree.cpp \
Culling/QuadTree/quad_tree_node.cpp \
Culling/QuadTree/quad_tree.cpp \
Culling/PortalMap/portal_clipping.cpp \
Culling/PortalMap/portal_map.cpp \
scene_light.cpp \
scene_camera.cpp \
scene_pass.cpp \
Model/model.cpp \
Model/model_instance.cpp \
Model/model_lod.cpp \
Model/model_shader_cache.cpp \
Model/model_cache.cpp \
Level/level.cpp \
scene_object.cpp \
Resources/scene_cache.cpp \
Framework/shader_setup.cpp \
Framework/instances_buffer.cpp \
Framework/material_cache.cpp \
Framework/shadow_maps.cpp \
Performance/gpu_timer.cpp \
Performance/scope_timer.cpp

libclan30Scene3D_la_LDFLAGS = \
  -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(LDFLAGS_LT_RELEASE) \
  $(extra_LIBS_clanScene3D)

libclan30Scene3D_la_CXXFLAGS=$(clanScene3D_CXXFLAGS) $(extra_CFLAGS_clanScene3D)
# EOF #

