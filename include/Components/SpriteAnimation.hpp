#pragma once

class SpriteAnimation {
public:
	unsigned int Index;
	unsigned int FrameCount;
	unsigned int AnimationSpeed;

	SpriteAnimation() {
		Index          = 0;
		FrameCount     = 0;
		AnimationSpeed = 0;
	}

	SpriteAnimation(unsigned int index, unsigned int frameCount,
									unsigned int animSpeed) {
		Index          = index;
		FrameCount     = frameCount;
		AnimationSpeed = animSpeed;
	}
};
