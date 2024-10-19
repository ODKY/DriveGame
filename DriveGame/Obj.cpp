#include "Obj.hpp"
#include "Player.hpp"

bool Obj::update() {
	// 当たり判定の位置調整
	int32 deltaX = camera.calc_delta_x(pos);
	auto posS = camera.world_pos_to_screen_pos(pos);
	hitbox.pos = { (int32)posS.x - hitbox.w / 2 + deltaX + boxOffset.x, (int32)posS.y + boxOffset.y };

	// 当たり判定
	if (hitbox.w != 0 && hitbox.intersects(player.get_hit_box())) {
		isHit = true;
		return false;
	}

	if (pos.z < camera.get_z())
		return false;
	//pos.z += 50;
	return true;
}
