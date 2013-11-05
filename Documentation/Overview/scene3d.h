
/*! \page Scene3D Scene3D Module Reference

	<h2>3D Scene Manager</h2>

	<ul>
		<li>clan::Scene - Main scene class</li>
		<li>clan::SceneCamera - A camera (point and direction to look from) in the scene</li>
		<li>clan::SceneLight - A dynamic light in the scene</li>
		<li>clan::SceneLightProbe - An object dynamic objects can sample static light from</li>
		<li>clan::SceneParticleEmitter - Particle system emitter (smoke, fire, etc) in the scene</li>
		<li>clan::SceneModel - 3D mesh model</li>
		<li>clan::SceneObject - Instance of a model in the scene (position, orientation, scale)</li>
		<li>clan::ScenePass - Interface for a custom pass to participate in the scene rendering</li>
	</ul>

	<h2>Performance Counters</h2>

	<ul>
		<li>clan::GPUTimer - Measures the duration of operations performed by the GPU</li>
		<li>clan::ScopeTimer - Measures the duration of operations performed by the CPU</li>
	</ul>

	<h2>3D Mesh Model Data Structures</h2>

	<ul>
		<li>clan::ModelData - Data structure describing a 3D mesh model</li>
		<li>clan::ModelDataMesh - Submeshes this model is made of</li>
		<li>clan::ModelDataTexture - Texture used by the model</li>
		<li>clan::ModelDataBone - Animation bone used by the model</li>
		<li>clan::ModelDataLight - Description of a light source attached to the model</li>
		<li>clan::ModelDataCamera - Description of a camera attached to the model</li>
		<li>clan::ModelDataAttachmentPoint - Points and orientation where other objects can be attached</li>
		<li>clan::ModelDataParticleEmitter - Description of particle emitters attached to the model</li>
		<li>clan::ModelDataAnimation - Animation timelines for the model</li>
	</ul>

	<h2>Indoor Portal Level Data Structures</h2>

	<ul>
		<li>clan::LevelData - Data structure describing areas connected by portals</li>
		<li>clan::LevelDataSector - An area in the level</li>
		<li>clan::LevelDataPortal - Portal connecting two sectors</li>
		<li>clan::LevelDataLight - Description of a light source in the level</li>
		<li>clan::LevelDataObject - Description of a 3D object in the level</li>
	</ul>

	<h2>Resources</h2>

	<ul>
		<li>clan::SceneCache - Resource cache interface for clanScene3D resources</li>
	</ul>

*/
