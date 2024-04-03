#pragma once

#include "Sprite.h"
#include "Vector4.h"
#include "TextureManager.h"


class  Particle :public Sprite{
public:
	 Particle();
	~ Particle();

	void Intilize();

	void Update();

	void Draw();




private:

	Sprite* sprite_ = nullptr;
	Sprite* BlendSprite_ = nullptr;

	uint32_t Texture_;
	uint32_t BlendTexture_;



};
