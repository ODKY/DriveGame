#include "Global.h"

// 乱数生成器
std::random_device seedGenerator;
std::mt19937 random;

// フォント
unique_ptr<Font> fontA;
unique_ptr<Font> fontDeb;

// ゲームステートマネージャー
unique_ptr<GameStateManager> gsm;

// 画像
unique_ptr<Texture> imgMountIwate;
unique_ptr<Texture> imgBallonR;
unique_ptr<Texture> imgTree01;
unique_ptr<Texture> imgTree02;
unique_ptr<Texture> imgGrass01;
unique_ptr<vector<TextureRegion>> imgRedCar;
unique_ptr<vector<TextureRegion>> imgBlackCar;
unique_ptr<vector<TextureRegion>> imgBlueCar;

// シェーダー
unique_ptr<VertexShader> vertexShader;
unique_ptr<PixelShader> pixelShader;

ConstantBuffer<TimeStruct> cbTime;
ConstantBuffer<RoadData> cbRoad;
ConstantBuffer<CameraData> cbCamera;
