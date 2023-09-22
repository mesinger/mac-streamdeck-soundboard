deploy:
	rm -rf build/
	mkdir build
	cd build; cmake -DCMAKE_BUILD_TYPE=Release ../plugin/
	cmake --build ./build/ --target soundboard -- -j 8
	mkdir build/plugin
	cp -a mesi.soundboard.sdPlugin/ ./build/plugin
	cp build/soundboard ./build/plugin
	cp -a propertyInspector/ ./build/plugin
	rm -rf ~/Library/Application\ Support/com.elgato.StreamDeck/Plugins/mesi.soundboard.sdPlugin/
	cp -R build/plugin/ ~/Library/Application\ Support/com.elgato.StreamDeck/Plugins/mesi.soundboard.sdPlugin/