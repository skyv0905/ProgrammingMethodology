#pragma once

#include "AL/al.h"
#include "AL/alc.h"
#include <vector>

class OpenALSoundHandler {
	/*-----------------------------------------------------Variables For Playing Music----------------------------------------------------------*/
public:
	// OpenAL 초기화
	ALCdevice* device;
	ALCcontext* context;

	// Wav 파일 로드하여 AudioData 저장하에 필요한 변수들
	std::vector<char> audioDataBackground;
	std::vector<char> audioDataBubblePopped;
	std::vector<char> audioDataBubbleShotted;
	std::vector<char> audioDataGameSucceeded;
	std::vector<char> audioDataGameOver;

	ALsizei sizebackground, frequencybackground;
	ALsizei sizepopped, frequencypopped;
	ALsizei sizeshotted, frequencyshotted;
	ALsizei sizesucceeded, frequencysucceeded;
	ALsizei sizeover, frequencyover;

	ALenum formatbackground;
	ALenum formatpopped;
	ALenum formatshotted;
	ALenum formatsucceeded;
	ALenum formatover;

	// 각 Wav 파일의 재생 상태 확인하는 변수들
	ALint sourceStateBackground;
	ALint sourceStatePopped;
	ALint sourceStateShotted;
	ALint sourceStateSucceeded;
	ALint sourceStateOver;

	// 각 Wav 파일의 재생을 위한 buffer와 source 변수들
	ALuint bufferbackground, sourcebackground;
	ALuint buffershotted, sourceshotted;
	ALuint buffersucceeded, sourcesucceeded;
	ALuint bufferover, sourceover;
	ALuint bufferpopped, sourcepopped;

	/*----------------------------------------Functions for Playing Music--------------------------------------------*/

	void playMusicBackground();
	void playMusicBubblePopped();
	void playMusicBubbleShotted();
	void playMusicGameSucceeded();
	void playMusicGameOver();
	void loadWavFile(const std::string& filename, std::vector<char>& buffer, ALsizei* size, ALsizei* frequency, ALenum* format);
	void setUpAudio();
	void cleanUpAudio();
};

void OpenALSoundHandler::playMusicBubblePopped() {

	alSourcePlay(sourcepopped);
}

void OpenALSoundHandler::playMusicBubbleShotted() {

	alSourcePlay(sourceshotted);
}

void OpenALSoundHandler::playMusicGameSucceeded() {

	alSourcePlay(sourcesucceeded);
}

void OpenALSoundHandler::playMusicGameOver() {

	alSourcePlay(sourceover);
}

void OpenALSoundHandler::playMusicBackground() {

	alSourcePlay(sourcebackground);
}

void OpenALSoundHandler::loadWavFile(const std::string& filename, std::vector<char>& buffer, ALsizei* size, ALsizei* frequency, ALenum* format) {

	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file");
	}

	char fileHeader[44];
	file.read(fileHeader, 44);

	if (std::strncmp(fileHeader, "RIFF", 4) != 0 || std::strncmp(fileHeader + 8, "WAVE", 4) != 0) {
		throw std::runtime_error("Not a valid WAV file");
	}

	*frequency = *(int*)(fileHeader + 24);
	int bitsPerSample = *(short*)(fileHeader + 34);
	int channels = *(short*)(fileHeader + 22);

	*format = (channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

	*size = *(int*)(fileHeader + 40);
	buffer.resize(*size);
	file.read(buffer.data(), *size);

	file.close();
}

void OpenALSoundHandler::setUpAudio() {

	/*------------------------- OpenAL 초기화 ---------------------------*/

	device = alcOpenDevice(NULL); // 기본 오디오 장치 열기

	if (!device) {
		std::cerr << "Unable to open default device" << std::endl;
	}

	context = alcCreateContext(device, NULL);

	if (!context) {
		std::cerr << "Unable to create context" << std::endl;
		alcCloseDevice(device);
	}

	alcMakeContextCurrent(context);

	/*------------------------- 오디오 데이터 로드 ---------------------------*/

	loadWavFile("Background.wav", audioDataBackground, &sizebackground, &frequencybackground, &formatbackground);
	loadWavFile("Bubble_Popped.wav", audioDataBubblePopped, &sizepopped, &frequencypopped, &formatpopped);
	loadWavFile("Bubble_Shotted.wav", audioDataBubbleShotted, &sizeshotted, &frequencyshotted, &formatshotted);
	loadWavFile("Game_Succeeded.wav", audioDataGameSucceeded, &sizesucceeded, &frequencysucceeded, &formatsucceeded);
	loadWavFile("Game_Over.wav", audioDataGameOver, &sizeover, &frequencyover, &formatover);

	// 버퍼 생성
	alGenBuffers(1, &bufferbackground);
	alGenBuffers(1, &bufferpopped);
	alGenBuffers(1, &buffershotted);
	alGenBuffers(1, &buffersucceeded);
	alGenBuffers(1, &bufferover);

	// 소스 생성
	alGenSources(1, &sourcebackground);
	alGenSources(1, &sourcepopped);
	alGenSources(1, &sourceshotted);
	alGenSources(1, &sourcesucceeded);
	alGenSources(1, &sourceover);

	// 소스에 버퍼 연결

	alBufferData(bufferbackground, formatbackground, audioDataBackground.data(), sizebackground, frequencybackground);
	alSourcei(sourcebackground, AL_BUFFER, bufferbackground);

	alBufferData(bufferpopped, formatpopped, audioDataBubblePopped.data(), sizepopped, frequencypopped);
	alSourcei(sourcepopped, AL_BUFFER, bufferpopped);

	alBufferData(buffershotted, formatshotted, audioDataBubbleShotted.data(), sizeshotted, frequencyshotted);
	alSourcei(sourceshotted, AL_BUFFER, buffershotted);

	alBufferData(buffersucceeded, formatsucceeded, audioDataGameSucceeded.data(), sizesucceeded, frequencysucceeded);
	alSourcei(sourcesucceeded, AL_BUFFER, buffersucceeded);

	alBufferData(bufferover, formatover, audioDataGameOver.data(), sizeover, frequencyover);
	alSourcei(sourceover, AL_BUFFER, bufferover);
}

void OpenALSoundHandler::cleanUpAudio() {

	alDeleteSources(1, &sourcebackground);
	alDeleteBuffers(1, &bufferbackground);

	alDeleteSources(1, &sourceover);
	alDeleteBuffers(1, &bufferover);

	alDeleteSources(1, &sourceshotted);
	alDeleteBuffers(1, &buffershotted);

	alDeleteSources(1, &sourcepopped);
	alDeleteBuffers(1, &bufferpopped);

	alDeleteSources(1, &sourcesucceeded);
	alDeleteBuffers(1, &buffersucceeded);

	alcDestroyContext(context);
	alcCloseDevice(device);
}