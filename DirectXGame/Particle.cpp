#include "Particle.h"

void Particle::Intilize() {



	Texture_ = TextureManager::Load("fog.png");
	BlendTexture_ = TextureManager::Load("fog2.png");

	sprite_->SetTextureHandle(Texture_);

	BlendSprite_->SetTextureHandle(BlendTexture_);

	sprite_->SetColor(SpriteColor);
	BlendSprite_->SetColor(BlendSpriteColor);

}

void Particle::Update() {}

void Particle::Draw() {



	sprite_->Draw();

	BlendMode::kAdd;

	BlendSprite_->Draw();

}
