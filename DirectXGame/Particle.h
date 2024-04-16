#pragma once

#include "Sprite.h"
#include "Vector4.h"
#include "TextureManager.h"
#include "ViewProjection.h"


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

	Vector4 SpriteColor = {1,1,1};
	Vector4 BlendSpriteColor = {1, 1, 1};

};
