#pragma once
#include <memory>

#include "QuestData.h"

namespace DAPI
{
  struct Quest
  {
    Quest() { data = std::make_shared<QuestData>(); }

    QuestID getID() { return data->id; }
    QuestState getState() { return data->state; }

    std::shared_ptr<QuestData> data;
  };
}