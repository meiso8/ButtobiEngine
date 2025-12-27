#include "CrowbarItem.h"
#include "SunRod.h"

std::shared_ptr<Item>  CrowbarItem::CombineWith(const std::shared_ptr<Item>& other)  {
    if (other->name_ == "SunMedal") {
        //メダルと合体するとSunRod
        return std::make_shared<SunRod>();
    }
    return nullptr;
}