//
// Created by uquark on 03/10/22.
//
#include <pocketsphinx.h>
#include "me_uquark_mcmagic_voice_recognize_ps4j_PS4J.h"

ps_decoder_t* decoder = NULL;
int32_t score = 0;

const char * KWS_NAME = "KWS_SPELLS";

JNIEXPORT void JNICALL Java_me_uquark_mcmagic_voice_recognize_ps4j_PS4J_init
        (JNIEnv * env, jclass cl, jstring model, jstring dict, jstring kws) {
    if (decoder)
        return;

    const char * cModel = (*env)->GetStringUTFChars(env, model, NULL);
    const char * cDict = (*env)->GetStringUTFChars(env, dict, NULL);
    const char * cKws = (*env)->GetStringUTFChars(env, kws, NULL);

    ps_config_t* config = ps_config_init(NULL);
    ps_config_set_str(config, "hmm", cModel);

    decoder = ps_init(config);

    ps_load_dict(decoder, cDict, NULL, NULL);
    ps_add_kws(decoder, KWS_NAME, cKws);

    ps_activate_search(decoder, KWS_NAME);

    printf("[NATIVE] PocketSphinx initialized\n");
    fflush(stdout);
}

JNIEXPORT jint JNICALL Java_me_uquark_mcmagic_voice_recognize_ps4j_PS4J_addWord
        (JNIEnv * env, jclass cl, jstring word, jstring phones, jboolean update) {
    if (!decoder)
        return -1;
    const char* cWord = (*env)->GetStringUTFChars(env, word, NULL);
    const char* cPhones = (*env)->GetStringUTFChars(env, phones, NULL);
    return ps_add_word(decoder, cWord, cPhones, update);
}

JNIEXPORT jint JNICALL Java_me_uquark_mcmagic_voice_recognize_ps4j_PS4J_process
        (JNIEnv * env, jclass cl, jbyteArray data, jlong samples, jboolean fullUtterance) {
    if (!decoder)
        return -1;
    const int16_t* cData = (const int16_t*) (*env)->GetByteArrayElements(env, data, NULL);
    return ps_process_raw(decoder, cData, samples, 0, fullUtterance);
}

JNIEXPORT jboolean JNICALL Java_me_uquark_mcmagic_voice_recognize_ps4j_PS4J_getInSpeech
        (JNIEnv * env, jclass cl) {
    if (!decoder)
        return 0;
    return ps_get_in_speech(decoder);
}

JNIEXPORT jstring JNICALL Java_me_uquark_mcmagic_voice_recognize_ps4j_PS4J_getHypothesis
        (JNIEnv * env, jclass cl) {
    if (!decoder)
        return NULL;
    const char* hypothesis = ps_get_hyp(decoder, &score);
    if (hypothesis)
        return (*env)->NewStringUTF(env, hypothesis);
    else
        return NULL;
}

JNIEXPORT jint JNICALL Java_me_uquark_mcmagic_voice_recognize_ps4j_PS4J_getScore
        (JNIEnv * env, jclass cl) {
    return score;
}

JNIEXPORT void JNICALL Java_me_uquark_mcmagic_voice_recognize_ps4j_PS4J_destroy
        (JNIEnv * env, jclass cl) {
    if (!decoder)
        return;
    ps_free(decoder);
    decoder = NULL;
}

JNIEXPORT jint JNICALL Java_me_uquark_mcmagic_voice_recognize_ps4j_PS4J_startUtterance
        (JNIEnv * env, jclass cl) {
    return ps_start_utt(decoder);
}

JNIEXPORT jint JNICALL Java_me_uquark_mcmagic_voice_recognize_ps4j_PS4J_endUtterance
        (JNIEnv * env, jclass cl) {
    return ps_end_utt(decoder);
}