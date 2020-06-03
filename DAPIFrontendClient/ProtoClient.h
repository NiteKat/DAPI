#pragma once
#include <iostream>

#include "../DAPILIB/source/Command.h"
#include "../DAPIBackendCore/DAPIProtoClient.h"
#include "../DAPILIB/source/Game.h"

namespace DAPI
{
  struct ProtoClient
  {
    friend struct Game;
    ProtoClient();

    bool isConnected() const;
    bool connect();
    void disconnect();
    void update(Game &game);
    bool issueCommand(Command command);

  private:
    bool hasCommand;
    void transmitMessages();
    DAPIProtoClient protoClient;
  };
}