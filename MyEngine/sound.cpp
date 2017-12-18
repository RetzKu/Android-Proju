#include "sound.h"
#include "sound_manager.h"

namespace Engine {
	namespace Audio {


		Sound::Sound(const std::string name, const std::string & filename)
			: m_Name(name), m_Filename(filename), m_Playing(false), m_ResumeLoop(false)
		{
			m_Sound = gau_load_sound_file(filename.c_str(), "wav");

		}
		Sound::~Sound()
		{
			ga_sound_release(m_Sound);
		}
		void Sound::play()
		{
			m_ResumeLoop = false;
			gc_int32 quit = 0;
			m_Handle = gau_create_handle_sound(SoundManager::m_Mixer, m_Sound, &setFlagAndDestroyOnFinish, &quit, NULL);
			m_Handle->sound = this;
			ga_handle_play(m_Handle);
			m_Playing = true;

		}
		void Sound::loop()
		{
			gc_int32 quit = 0;
			m_Handle = gau_create_handle_sound(SoundManager::m_Mixer, m_Sound, &LoopOnFinish, &quit, NULL);
			m_Handle->sound = this;
			ga_handle_play(m_Handle);
			m_Playing = true;
			m_ResumeLoop = true;
		}
		void Sound::resume()
		{
			gc_int32 quit = 0;
			ga_handle_play(m_Handle);

		}
		void Sound::pause()
		{
			m_Playing = false;
			ga_handle_stop(m_Handle);

		}
		void Sound::stop()
		{
			ga_handle_stop(m_Handle);
			m_Playing = false;
		}
		void setFlagAndDestroyOnFinish(ga_Handle* in_handle, void* in_context)
		{
			Sound* sound = (Sound*)in_handle->sound;
			sound->m_Playing = false;
			gc_int32* flag = (gc_int32*)(in_context);
			*flag = 1;
			ga_handle_destroy(in_handle);
		}


		void LoopOnFinish(ga_Handle* in_handle, void* in_context)
		{
			Sound* sound = (Sound*)in_handle->sound;
			sound->loop();
			ga_handle_destroy(in_handle);
		}
		void Sound::setGain(float gain)
		{
			if (!m_Playing)
			{
				//std::cout << "Cannot set gain! Sound is not currently playing!" << std::endl;
			}
			m_Gain = gain;
			ga_handle_setParamf(m_Handle, GA_HANDLE_PARAM_GAIN, gain);
		}
	}
}