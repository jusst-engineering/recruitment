/*
 * Copyright (c) 2016, Peter Thorson. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the WebSocket++ Project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL PETER THORSON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using nlohmann::json; 

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

// This message handler will be invoked once for each incoming message. 
void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {

	/* 
		Ein paar Worte zur Vorgehenesweise

		Ich benutze hauptsächlich die Library "websocketpp", womit ich die Datenpakete annehme. Boost ist eine Abhängigkeit von websocketpp. Die Methode on_message wird jedes Mal aufgerufen, wenn eine neue Message ankommt. Daraufhin liegt das reine Payload bereits vor und kann weiterverarbeitet werden, hierzu verwende ich die JSON-LIbrary "json". Mit Hilfe des Strings "payload" kann ein JSON-Objekt initliaisiert werden und die entsprechenden JSON-Library-Befehle sind anwendbar. Ich printe immer das reine Payload, um zu sehen, ob meine ausgegebenen Meldungen stimmen. Die nachfolgenden Idee ist, dass pro abzubildenen Systemstate eine if die entsprechende std::cout-Ausgabe "triggert". Der Übersicher halber hab ich alle Bools oben deklariert und initialisiert, ebenso das Volume, was für die eine Ausgabe wichtig ist. Die nachfolgenden Ifs schauen, ob zu dem entsprechendem Schlüsselwort bzw. Key auch der gesuchte Value exisitiert und beschreiben dann einen obigen definierten Boolean. Der Grund, warum immer das Contains davor ist, ist, dass es bei der JSON-Library keinen Default-Wert gibt, d.h. wenn ein Datenpaket z.B. kein "system" enthält, wird eine Exception geworfen, weil der String null ist. Das kann verhindert werden, wenn man vorher schaut, ob der Key überhaupt im Datenpaket enthalten ist, sieht allerdings nicht so schön aus und ich habe auch sehr lange nach einer Lösung gesucht auf der GitHub-Issue-Seite von nlohmann/json. Pro Boolean gibt es eben eine passende If, die dann die std::cout-Ausgabe triggert. Bei System-Error z.B. ist das nicht besonders spannend, allerdings muss z.B. für white@volume der Volume-Int extrahiert werden. Ebenso muss für die Ausgabe blue@10 sich jeweils der State gemerkt werden, wenn Bluetooth connectet ist UND ein Song spielt, da es nicht vorkommt, dass beides in einem Paket passiert.
	*/

	// Payload
	std::string payload(msg->get_payload());
	auto j = json::parse(payload);

	// Print payload
	std::cout << j << std::endl;

	bool syserror = false;
	bool sysbooting = false;
	bool sysupdating = false;
	bool playbackinactive = false;
	bool playbackplayingandbluetoothconnected = false;
	bool playbackplaying = false;
	bool playbackpaused = false;
	bool playbackvolumechanged = false;
	bool bluetoothpairing = false;
	bool bluetoothpaired = false;

	int volume = 0;

	if(j.contains("system")) {
	  sysbooting = j["system"].get<std::string>() == "booting";
	}

	if(j.contains("system")) {
	  sysupdating = j["system"].get<std::string>() == "updating";
	}

	if(j.contains("system")) {
	  syserror = j["system"].get<std::string>() == "error";
	}

	if(j.contains("volume")) {
	  volume = j["volume"].get<int>();
	  // If volume stays 50 for some reason it didn't change, but I consider this as a change as well
	  playbackvolumechanged = true;
	}

	if(j.contains("playback")) {
		playbackinactive = j["playback"].get<std::string>() == "inactive";
		if(playbackinactive) playbackplaying = false;
	}

	if(j.contains("playbackPosition")) {
		playbackplaying = true;
	}

	if(j.contains("playback")) {
		playbackpaused = j["playback"].get<std::string>() == "paused";
	}

	if(j.contains("bluetooth")) {
		// There is no "paired" message, so I assume after no error message after pairing the device is paired
		bluetoothpairing = j["bluetooth"].get<std::string>() == "pairing";
		bluetoothpaired = true;
	
	} 

	if(playbackplaying && bluetoothpaired) {
		// First occurence of playbackPosition indicates if a song is playing
		playbackplayingandbluetoothconnected = true;
		std::cout << "yooooooooooooooooooooooooooooo" << std::endl;
	} else {

		//playbackplayingandbluetoothconnected = false;
	}

	// System outputs
	if(syserror) {

		std::cout << "red@100" << std::endl;
	} 

	if(sysupdating) {

		std::cout << "off" << std::endl;
		std::cout << "yellow@100" << std::endl;
	} 

	if(sysbooting) {

		std::cout << "red@10" << std::endl;
	} 

	if(bluetoothpairing) {

		std::cout << "off" << std::endl;
		std::cout << "blue@100" << std::endl;
		bluetoothpairing = false;
	}  

	if(playbackvolumechanged) {

		std::cout << "white@" << volume << " for 3s" << std::endl;

		// TODO fade

		playbackvolumechanged = false;
	} 

	if(playbackinactive) {

		std::cout << "off" << std::endl;
	} 

	if(playbackplayingandbluetoothconnected) {

		std::cout << "blue@10" << std::endl;

	} 

	if(playbackplaying) {

		std::cout << "white@10" << std::endl;
		playbackplaying = false;
	} 

	if(playbackpaused) {

		std::cout << "white@50" << std::endl;

	} 

	websocketpp::lib::error_code ec;

    //c->send(hdl, msg->get_payload(), msg->get_opcode(), ec);

    if (ec) {
        std::cout << "Echo failed because: " << ec.message() << std::endl;
    }
}

int main(int argc, char* argv[]) {

    // Create a client endpoint
    client c;

    std::string uri = "ws://0.0.0.0:8808/ws";

    if (argc == 2) {
        uri = argv[1];
    }

    try {
        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        c.init_asio();

        // Register our message handler
        c.set_message_handler(bind(&on_message,&c,::_1,::_2));

        websocketpp::lib::error_code ec;

        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 0;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        c.connect(con);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        c.run();

    } catch (websocketpp::exception const & e) {

        std::cout << "exception catchhhhhh" << std::endl;
        std::cout << e.what() << std::endl;
    }
}

