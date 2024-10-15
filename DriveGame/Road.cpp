#include "Road.h"

void Road::draw() const {
	renderTexture.clear(Palette::Black);
	{
		const ScopedRenderTarget2D renderTarget{ renderTexture };
		const Point offset{ RENDER_TEXTURE_W_QUARTER, 0 };

		// 道路
		Vec3 roadNearL{ POS_X - roadWidth / 2.0, POS_Y, camera.get_z() };
		Vec3 roadNearR{ POS_X + roadWidth / 2.0, POS_Y, camera.get_z() };
		Vec3 roadFarL = roadNearL.movedBy(0.0, 0.0, FAR_Z);
		Vec3 roadFarR = roadNearR.movedBy(0.0, 0.0, FAR_Z);
		Quad{
			camera.world_pos_to_screen_pos(roadNearL) + offset,
			camera.world_pos_to_screen_pos(roadFarL) + offset,
			camera.world_pos_to_screen_pos(roadFarR) + offset,
			camera.world_pos_to_screen_pos(roadNearR) + offset
		}.draw(ROAD_COLOR);

		// 左線（内側）のベース
		Vec3 leftInNearL{ POS_X - roadWidth / 2.0 + SIDE_LINE_IN_PADDING, POS_Y, camera.get_z() };
		Vec3 leftInNearR{ leftInNearL.x + SIDE_LINE_IN_WIDTH, POS_Y, camera.get_z() };
		Vec3 leftInFarL = leftInNearL.movedBy(0.0, 0.0, FAR_Z);
		Vec3 leftInFarR = leftInNearR.movedBy(0.0, 0.0, FAR_Z);
		Quad{
			camera.world_pos_to_screen_pos(leftInNearL) + offset,
			camera.world_pos_to_screen_pos(leftInFarL) + offset,
			camera.world_pos_to_screen_pos(leftInFarR) + offset,
			camera.world_pos_to_screen_pos(leftInNearR) + offset
		}.draw(SIDE_LINE_IN_COLOR_A);

		// 右線（内側）のベース
		Vec3 rightInNearR{ POS_X + roadWidth / 2.0 - SIDE_LINE_IN_PADDING, POS_Y, camera.get_z() };
		Vec3 rightInNearL{ rightInNearR.x - SIDE_LINE_IN_WIDTH, POS_Y, camera.get_z() };
		Vec3 rightInFarR = rightInNearR.movedBy(0.0, 0.0, FAR_Z);
		Vec3 rightInFarL = rightInNearL.movedBy(0.0, 0.0, FAR_Z);
		Quad{
			camera.world_pos_to_screen_pos(rightInNearL) + offset,
			camera.world_pos_to_screen_pos(rightInFarL) + offset,
			camera.world_pos_to_screen_pos(rightInFarR) + offset,
			camera.world_pos_to_screen_pos(rightInNearR) + offset
		}.draw(SIDE_LINE_IN_COLOR_A);

		// 左線（外側）のベース
		Vec3 leftOutNearL{ POS_X - roadWidth / 2.0, POS_Y, camera.get_z() };
		Vec3 leftOutNearR{ leftOutNearL.x + SIDE_LINE_OUT_WIDTH, POS_Y, camera.get_z() };
		Vec3 leftOutFarL = leftOutNearL.movedBy(0.0, 0.0, FAR_Z);
		Vec3 leftOutFarR = leftOutNearR.movedBy(0.0, 0.0, FAR_Z);
		Quad{
			camera.world_pos_to_screen_pos(leftOutNearL) + offset,
			camera.world_pos_to_screen_pos(leftOutFarL) + offset,
			camera.world_pos_to_screen_pos(leftOutFarR) + offset,
			camera.world_pos_to_screen_pos(leftOutNearR) + offset
		}.draw(SIDE_LINE_OUT_COLOR);

		// 右線（外側）のベース
		Vec3 rightOutNearR{ POS_X + roadWidth / 2.0, POS_Y, camera.get_z() };
		Vec3 rightOutNearL{ rightOutNearR.x - SIDE_LINE_OUT_WIDTH, POS_Y, camera.get_z() };
		Vec3 rightOutFarR = rightOutNearR.movedBy(0.0, 0.0, FAR_Z);
		Vec3 rightOutFarL = rightOutNearL.movedBy(0.0, 0.0, FAR_Z);
		Quad{
			camera.world_pos_to_screen_pos(rightOutNearL) + offset,
			camera.world_pos_to_screen_pos(rightOutFarL) + offset,
			camera.world_pos_to_screen_pos(rightOutFarR) + offset,
			camera.world_pos_to_screen_pos(rightOutNearR) + offset
		}.draw(SIDE_LINE_OUT_COLOR);


		// 中央線描画
		for (int i = 0; i < CENTER_LINE_NUM; ++i) {

			// 線の長さを L としたときに、2L - (進行距離 % 2L) + 進行距離 - L を線のスタートとしている
			// 最後の -L はよくわからないが無いとだめ
			// 進行距離に足す値を徐々に減らすことで、カメラとの相対距離を減らす、つまり近づくようにしている
			// 余りの計算は少数だとできないため、rate倍して整数化してから求めている
			// 0で割らないよう、tmpを用意した
			const double rate = 10000.0;
			int32 tmp = (int32)(CENTER_LINE_LENGTH * 2 * rate);
			if (tmp == 0)
				tmp = 1;
			const double remainder = (int32)(camera.get_z() * rate) % tmp / rate;
			const double start = CENTER_LINE_LENGTH * 2 - remainder + camera.get_z() - CENTER_LINE_LENGTH;
			const double z = start + CENTER_LINE_LENGTH * i * 2;

			// 中央線の頂点ワールド座標
			const Vec3 centerNearL{ POS_X - CENTER_LINE_WIDTH / 2.0, POS_Y, z };
			const Vec3 centerNearR{ POS_X + CENTER_LINE_WIDTH / 2.0, POS_Y, z };
			const Vec3 centerFarL = centerNearL.movedBy(0.0, 0.0, CENTER_LINE_LENGTH);
			const Vec3 centerFarR = centerNearR.movedBy(0.0, 0.0, CENTER_LINE_LENGTH);

			// 中央線描画
			Quad{
				camera.world_pos_to_screen_pos(centerNearL) + offset,
				camera.world_pos_to_screen_pos(centerFarL) + offset,
				camera.world_pos_to_screen_pos(centerFarR) + offset,
				camera.world_pos_to_screen_pos(centerNearR) + offset
			}.draw(CENTER_LINE_COLOR);
		}

		// 両脇（内側）の線
		for (int i = 0; i < SIDE_LINE_IN_NUM; ++i) {

			const double rate = 10000.0;
			int32 tmp = (int32)(SIDE_LINE_IN_LENGTH * 2 * rate);
			if (tmp == 0)
				tmp = 1;
			const double remainder = (int32)(camera.get_z() * rate) % tmp / rate;
			const double start = SIDE_LINE_IN_LENGTH * 2 - remainder + camera.get_z() - SIDE_LINE_IN_LENGTH;
			const double z = start + SIDE_LINE_IN_LENGTH * i * 2;

			// 左線の頂点ワールド座標
			const Vec3 leftNearL{ POS_X - roadWidth / 2.0 + SIDE_LINE_IN_PADDING, POS_Y, z };
			const Vec3 leftNearR{ leftNearL.x + SIDE_LINE_IN_WIDTH, POS_Y, z };
			const Vec3 leftFarL = leftNearL.movedBy(0.0, 0.0, SIDE_LINE_IN_LENGTH);
			const Vec3 leftFarR = leftNearR.movedBy(0.0, 0.0, SIDE_LINE_IN_LENGTH);

			// 左線描画
			Quad{
				camera.world_pos_to_screen_pos(leftNearL) + offset,
				camera.world_pos_to_screen_pos(leftFarL) + offset,
				camera.world_pos_to_screen_pos(leftFarR) + offset,
				camera.world_pos_to_screen_pos(leftNearR) + offset
			}.draw(SIDE_LINE_IN_COLOR_B);

			// 右線の頂点ワールド座標
			const Vec3 rightNearR{ POS_X + roadWidth / 2.0 - SIDE_LINE_IN_PADDING, POS_Y, z };
			const Vec3 rightNearL{ rightNearR.x - SIDE_LINE_IN_WIDTH, POS_Y, z };
			const Vec3 rightFarR = rightNearR.movedBy(0.0, 0.0, SIDE_LINE_IN_LENGTH);
			const Vec3 rightFarL = rightNearL.movedBy(0.0, 0.0, SIDE_LINE_IN_LENGTH);

			// 右線描画
			Quad{
				camera.world_pos_to_screen_pos(rightNearL) + offset,
				camera.world_pos_to_screen_pos(rightFarL) + offset,
				camera.world_pos_to_screen_pos(rightFarR) + offset,
				camera.world_pos_to_screen_pos(rightNearR) + offset
			}.draw(SIDE_LINE_IN_COLOR_B);
		}
	}

	{
		cbTime->time = (float)Scene::Time();
		Graphics2D::SetPSConstantBuffer(1, cbTime);
		Graphics2D::SetPSConstantBuffer(2, cbRoad);
		const ScopedCustomShader2D shader{ *vertexShader, *pixelShader };
		renderTexture.draw(-RENDER_TEXTURE_W_QUARTER, 0);
	}

	//auto pos = camera.world_pos_to_screen_pos({ 0, 0, 3 });
	auto pos = camera.camera_pos_to_screen_pos({ 0, 145, 3 });
	Line{ pos.x, pos.y, pos.x + 1300, pos.y }.draw(Palette::Red);
}
