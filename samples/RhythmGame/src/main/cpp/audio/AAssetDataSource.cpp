/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <utils/logging.h>
#include "AAssetDataSource.h"


AAssetDataSource* AAssetDataSource::newFromAssetManager(AAssetManager *assetManager,
                                                        const char *filename,
                                                        const int32_t channelCount) {

    // Load the backing track
    AAsset* asset = AAssetManager_open(assetManager, filename, AASSET_MODE_BUFFER);

    if (asset == nullptr){
        LOGE("Failed to open track, filename %s", filename);
        return nullptr;
    }

    // Get the length of the track (we assume it is stereo 48kHz)
    off_t trackLength = AAsset_getLength(asset);

    // Load it into memory
    auto *audioBuffer = static_cast<const int16_t*>(AAsset_getBuffer(asset));

    if (audioBuffer == nullptr){
        LOGE("Could not get buffer for track");
        return nullptr;
    }

    // There are 4 bytes per frame because
    // each sample is 2 bytes and
    // it's a stereo recording which has 2 samples per frame.
    int32_t numFrames = static_cast<int32_t>(trackLength / 4);
    LOGD("Opened backing track, bytes: %ld frames: %d", trackLength, numFrames);

    return new AAssetDataSource(asset, audioBuffer, numFrames, channelCount);
}
