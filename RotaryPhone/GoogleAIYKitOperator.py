#!/usr/bin/env python3
# Copyright 2017 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Run a recognizer using the Google Assistant Library with button support.

The Google Assistant Library has direct access to the audio API, so this Python
code doesn't need to record audio. Hot word detection "OK, Google" is supported.

It is available for Raspberry Pi 2/3 only; Pi Zero is not supported.
"""

import logging
import platform
import sys
import threading
import time
import os
from time import sleep
import random

from google.assistant.library.event import EventType

from aiy.assistant import auth_helpers
from aiy.assistant.library import Assistant
from aiy.board import Board, Led
import aiy.voice.audio

def greeting():
    randomgreeting = random.choice(os.listdir("/etc/custom-scripts/sfx/greeting/"))
    greetingfile = '/etc/custom-scripts/sfx/greeting/' + randomgreeting
    logging.info(greetingfile)
    aiy.voice.audio.play_wav(greetingfile)
    def goodbye():
    randomgoodbye = random.choice(os.listdir("/etc/custom-scripts/sfx/goodbye/"))
    goodbyefile = '/etc/custom-scripts/sfx/goodbye/' + randomgoodbye
    logging.info(goodbyefile)
    aiy.voice.audio.play_wav(goodbyefile)

def unavailable():
    randomunavailable = random.choice(os.listdir("/etc/custom-scripts/sfx/unavailable/"))
    unavailablefile = '/etc/custom-scripts/sfx/unavailable/' + randomunavailable
    logging.info(unavailablefile)
    aiy.voice.audio.play_wav(unavailablefile)

class MyAssistant:

    def __init__(self):
        self._task = threading.Thread(target=self._run_task)
        self._can_start_conversation = False
        self._assistant = None
        self._board = Board()
        self._board.language_code = 'en-US'
        self._board.volume_percentage = 100
#       which one of these to uncomment depends on your phone inner workings
        self._board.button.when_pressed = self._on_button_pressed
#        self._board.button.when_released = self._on_button_released

    def start(self):
        self._task.start()

    def _run_task(self):
        credentials = auth_helpers.get_assistant_credentials()
        with Assistant(credentials) as assistant:
            self._assistant = assistant
            for event in assistant.start():
                self._process_event(event)

    def _process_event(self, event):
        logging.info(event)
        if event.type == EventType.ON_START_FINISHED:
            self._board.led.status = Led.OFF 
            self._can_start_conversation = True

        elif event.type == EventType.ON_CONVERSATION_TURN_STARTED:
            self._can_start_conversation = False
            self._board.led.state = Led.ON

        elif event.type == EventType.ON_END_OF_UTTERANCE:
            self._board.led.state = Led.PULSE_QUICK

        elif (event.type == EventType.ON_CONVERSATION_TURN_FINISHED
              or event.type == EventType.ON_CONVERSATION_TURN_TIMEOUT
              or event.type == EventType.ON_NO_RESPONSE):
            time.sleep(2)
            goodbye()
            self._board.led.state = Led.OFF 
            self._can_start_conversation = True

        elif event.type == EventType.ON_ASSISTANT_ERROR and event.args and event.args['is_fatal']:
            sys.exit(1)

    def _operator(self):
        greeting()
        self._assistant.start_conversation()

#       which one of these to uncomment depends on your phone inner workings        
    def _on_button_pressed(self):
#    def _on_button_released(self):
        answer = [self._operator, self._operator, self._operator, self._operator, unavailable]

        if self._can_start_conversation:
            random.choice(answer)()

def main():
    logging.basicConfig(level=logging.INFO)
    MyAssistant().start()
    
if __name__ == '__main__':
    main()
