#include <pch/pch.hpp>
#ifdef HP_DEBUG
#include <vld.h>
#endif
#include <hpIp.hpp>
using namespace hp_ip;
int main( )
{
	Actor ball( { // transform
		{ 0.0f, 0.45f, 0.0f }, // pos
		{ 0.0f, 0.0f, 0.0f }, // vel
		{ 1.0f, 1.0f, 1.0f }, // scl
		FQuat::identity, // rot
		FQuat::identity // modelRot
	} );
	auto ballModel = ModelComponent( // model
		"assets/models/basketball/basketball.fbx", // filename
		0.02f, // scale
		MaterialDef{ // material
		"assets/textures/basketball/basketball-diffuse.jpg", // diffuseTextureFilename
		"", // specularTextureFilename
		"assets/textures/basketball/basketball-bump.jpg", // bumpTextureFilename
		"", // parallaxTextureFilename
		"" // evnMapTextureFilename
	} );
	ball.addComponent( &ballModel );
	Actor camera( { // transform
		{ 0.0f, 2.0f, -7.0f }, // pos
		{ 0.0f, 0.0f, 0.0f }, // vel
		{ 1.0f, 1.0f, 1.0f }, // scl
		FQuat::identity, // rot
		FQuat::identity // modelRot
	} );
	auto cameraComp = CameraComponent( { // camera
		0.001f, // nearClipDist
		1000.0f, // farClipDist
	} );
	camera.addComponent( &cameraComp );
	ball.addChild( std::move( camera ) );
	Actor ground( { // transform
		{ 0.0f, 0.0f, 0.0f }, // pos
		{ 0.0f, 0.0f, 0.0f }, // vel
		{ 1.0f, 1.0f, 1.0f }, // scl
		FQuat::identity, // rot
		FQuat::identity // modelRot
	} );
	auto groundModel = ModelComponent( // model
		BuiltInModelType::Box, // type
		{ 500.0f, 0.1f, 500.0f }, // dimensions
		MaterialDef{ // material
		"assets/textures/ground/OrangeHerringbone-ColorMap.png", // diffuseTextureFilename
		"", // specularTextureFilename
		"assets/textures/ground/OrangeHerringbone-NormalMap.png", // bumpTextureFilename
		"", // parallaxTextureFilename
		"", // evnMapTextureFilename
		{ 250.0f, 250.0f } // textureRepeat
	} );
	ground.addComponent( &groundModel );
	Engine engine{ "example1" };
	engine.addActor( std::move( ball ) );
	engine.addActor( std::move( ground ) );
	engine.run( );
	return 0;
}