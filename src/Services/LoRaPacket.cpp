#include "LoRaPacket.h"

size_t MessagePacket::pack(void** destination) const{
	size_t size = sizeof(uid) + sizeof(type) + sizeof(timestamp);
	uint8_t* buffer = static_cast<uint8_t*>(malloc(size));

	memcpy(buffer, &type, sizeof(type));
	memcpy(buffer + sizeof(type), &uid, sizeof(uid));
	memcpy(buffer + sizeof(type) + sizeof(uid), &timestamp, sizeof(timestamp));

	*destination = buffer;
	return size;
}

MessagePacket* MessagePacket::unpack(void* _buffer){
	uint8_t* buffer = static_cast<uint8_t*>(_buffer);

	MessagePacket* packet;
	Type type = *reinterpret_cast<Type*>(buffer);

	if(type == ACK || type == READ){
		packet = new MessagePacket;
		packet->type = type;
	}else if(type == TEXT){
		packet = TextMessage::unpack(buffer + sizeof(type) + sizeof(uid) + sizeof(timestamp));
	}else if(type == PIC){
		packet = PicMessage::unpack(buffer + sizeof(type) + sizeof(uid) + sizeof(timestamp));
	}else{
		return nullptr;
	}

	packet->type = type;
	packet->uid = *reinterpret_cast<UID_t*>(buffer + sizeof(Type));
	packet->timestamp = *reinterpret_cast<uint32_t*>(buffer + sizeof(Type) + sizeof(UID_t));

	return packet;
}

TextMessage::TextMessage(){
	type = TEXT;
	timestamp = 0;
}

TextMessage::TextMessage(const std::string &text) : TextMessage(){
	this->text = text;
}

size_t TextMessage::pack(void** destination) const{
	uint8_t* buffer;
	size_t size = MessagePacket::pack(reinterpret_cast<void**>(&buffer));
	std::string text = this->text.substr(0, 60);

	size_t totalSize = size + 1 + text.size();
	buffer = static_cast<uint8_t*>(realloc(buffer, totalSize));

	*(buffer + size) = text.size();
	memcpy(buffer + size + 1, text.c_str(), text.size());

	*destination = buffer;
	return totalSize;
}

TextMessage* TextMessage::unpack(void* _buffer){
	uint8_t* buffer = static_cast<uint8_t*>(_buffer);
	TextMessage* message = new TextMessage;

	uint8_t size = *buffer;
	message->text = std::string(reinterpret_cast<char*>(buffer + 1), size);

	return message;
}

PicMessage::PicMessage(){
	type = PIC;
	timestamp = 0;
}

PicMessage::PicMessage(uint8_t index) : PicMessage(){
	this->index = index;
}

size_t PicMessage::pack(void** destination) const{
	uint8_t* buffer;
	size_t size = MessagePacket::pack(reinterpret_cast<void**>(&buffer));

	buffer = static_cast<uint8_t*>(realloc(buffer, size + 1));
	memcpy(buffer + size, &index, 1);

	*destination = buffer;
	return size + 1;
}

PicMessage* PicMessage::unpack(void* _buffer){
	PicMessage* message = new PicMessage;
	message->index = *static_cast<uint8_t*>(_buffer);
	return message;
}

AdvertisePair* AdvertisePair::unpack(void* _buffer){
	auto advert = new AdvertisePair;
	advert->profile = *static_cast<Profile*>(_buffer);
	return advert;
}

size_t AdvertisePair::pack(void** destination) const{
	uint8_t* buffer;

	buffer = static_cast<uint8_t*>(realloc(buffer, sizeof(Profile)));
	memcpy(buffer, &profile, sizeof(Profile));

	*destination = buffer;
	return sizeof(Profile);
}

AdvertisePair::AdvertisePair(const Profile &prof) : AdvertisePair(){
	profile = prof;
}

RequestPair* RequestPair::unpack(void* buffer){
	auto pairPacket = new RequestPair;
	memcpy(pairPacket->encKey, buffer, sizeof(encKey));
	return pairPacket;
}

size_t RequestPair::pack(void** destination) const{
	uint8_t* buffer;

	buffer = static_cast<uint8_t*>(realloc(buffer, sizeof(encKey)));
	memcpy(buffer, &encKey, sizeof(encKey));

	*destination = buffer;
	return sizeof(encKey);
}
