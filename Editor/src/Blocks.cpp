#include "Editor/Blocks.h"
#include "Editor/Scene.h"

using namespace Editor;

BlueBlock::BlueBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().BlueBlock} {}

GreenBlock::GreenBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().GreenBlock} {}

CyanBlock::CyanBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().CyanBlock} {}

OrangeBlock::OrangeBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().OrangeBlock} {}

RedBlock::RedBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().RedBlock} {}

YellowBlock::YellowBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().YellowBlock} {}

StrongYellowBlock::StrongYellowBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().StrongYellowBlock} {}

StrongRedBlock::StrongRedBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().StrongRedBlock} {}

CrackedYellowBlock::CrackedYellowBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().CrackedYellowBlock} {}

CrackedRedBlock::CrackedRedBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().CrackedRedBlock} {}

TNTRedBlock::TNTRedBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().TNTRedBlock} {}

TNTBlueBlock::TNTBlueBlock(
  Scene& ParentScene, SDL_Rect Rect)
  : Actor{
    ParentScene,
    SDL_Rect{Rect.x, Rect.y, WIDTH, HEIGHT},
    ParentScene.GetAssets().TNTBlueBlock} {}
