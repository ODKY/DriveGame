#include "Global.h"

// 乱数生成器
std::random_device seedGenerator;
std::mt19937 random;

// フォント
unique_ptr<Font> fontA;
unique_ptr<Font> fontB;
unique_ptr<Font> fontDeb;

// ゲームステートマネージャー
unique_ptr<GameStateManager> gsm;

bool isStart = false;
double countDown = 4.4;
bool aaa = false;
bool isGoal = false;

// 画像
unique_ptr<Texture> imgMountIwate;
unique_ptr<Texture> imgBallonR;
unique_ptr<Texture> imgTree01;
unique_ptr<Texture> imgTree02;
unique_ptr<Texture> imgGrass01;
unique_ptr<vector<TextureRegion>> imgRedCar;
unique_ptr<vector<TextureRegion>> imgBlackCar;
unique_ptr<vector<TextureRegion>> imgBlueCar;
unique_ptr<vector<TextureRegion>> imgObjects;

// シェーダー
unique_ptr<VertexShader> vertexShader;
unique_ptr<PixelShader> pixelShader;

ConstantBuffer<TimeStruct> cbTime;
ConstantBuffer<RoadData> cbRoad;
ConstantBuffer<CameraData> cbCamera;

double curveAmount = 0;

bool isHit;
bool isDisplayHitBox = false;
