#include "Fighter.h"

#include "ClientInfo.h"
#include "Messages_defs.h"

Fighter::Fighter(msg::ObjectId id, SDLGame* game)
    : Container(game),  //
      fighterImage_(game->getServiceLocator()->getTextures()->getTexture(
                        Resources::Airplanes),
                    {47, 90, 207, 250}),
      normalGun_(SDLK_SPACE) {
  setWidth(40);
  setHeight(50);
  setId(id);
  setActive(false);

  addC(&fighterImage_);
  if ((ClientInfo::instance()->getClientId() == 0 &&
       getId() == msg::Fighter_0) ||
      (ClientInfo::instance()->getClientId() == 1 &&
       getId() == msg::Fighter_1)) {
    addC(&naturalMove_);
    addC(&rotation_);
    addC(&oppositeSide_);
    addC(&normalGun_);
    addC(&thrust_);
    addC(&reduceSpeed_);
    initFighter();
  }
}

Fighter::~Fighter() {}

void Fighter::receive(const void* senderObj, const msg::Message& msg) {
  Container::receive(senderObj, msg);  // to send to all components

  switch (msg.type_) {
    case msg::MessageType::REMOTE_OBJ_INFO: {
      if (ClientInfo::instance()->getClientId() != msg.clientId_ &&
          getId() == msg.sender_) {
        const auto message =
            reinterpret_cast<const msg::RemoteObjectInfo&>(msg);
        setPosition({message.posX_, message.posY_});
        setVelocity({message.velX_, message.velY_});
        setRotation(message.rotation_);
      }
    }
    case msg::MessageType::GAME_START: {
      setActive(true);
      break;
    }
    case msg::MessageType::STOP_GAME:
    case msg::MessageType::GAME_OVER: {
      setActive(false);
      break;
    }
    default:
      break;
  }
}

void Fighter::initFighter() {
  setVelocity(Vector2D(0.0, 0.0));

  if (getId() == msg::Fighter_0) {
    setPosition(Vector2D(getGame()->getWindowWidth() / 2 - getWidth() / 2,
                         getGame()->getWindowHeight() / 2 - getHeight() / 2));
  } else {
    setPosition(Vector2D(getGame()->getWindowWidth() / 2 + getWidth() / 2,
                         getGame()->getWindowHeight() / 2 - getHeight() / 2));
  }
}
