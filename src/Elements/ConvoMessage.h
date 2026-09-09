#ifndef CHATTER_FIRMWARE_CONVOMESSAGE_H
#define CHATTER_FIRMWARE_CONVOMESSAGE_H

#include <string>
#include "../Interface/LVObject.h"
#include "../Model/Message.h"

class ConvoMessage : public LVObject{
public:
	ConvoMessage(lv_obj_t* parent, const Message& msg, uint16_t bgColor);
	virtual ~ConvoMessage();
	void setDeliveryStatus(bool delivered, bool read, bool failed);
	void setHue(uint16_t hue);

	const Message& getMsg() const;
	void clearFocus();

private:
	lv_style_t defaultStyle;
	lv_style_t focusedStyle;
	lv_obj_t* deliveredIndicator = nullptr;
	lv_obj_t* readIndicator = nullptr;
	lv_obj_t* failedIndicator = nullptr;
	lv_obj_t* label = nullptr;
	lv_obj_t* timestampLabel = nullptr;

	Message msg;
};


#endif //CHATTER_FIRMWARE_CONVOMESSAGE_H
