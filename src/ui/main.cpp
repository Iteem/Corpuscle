#include <iostream>

#include <imgui.h>
#include <imgui-sfml.h>
#include <SFML/Graphics.hpp>

#include "renderer/rendermanager.hpp"


const unsigned int guiSize = 250;
const unsigned int numThreads = 4;

int main()
{
	sf::Vector2u windowSize( 1280, 720 );
	sf::Vector2u renderSize( windowSize );

	// Window.
	sf::RenderWindow window( sf::VideoMode(windowSize.x, windowSize.y), "Corpuscle", sf::Style::Titlebar | sf::Style::Close );
	window.setVerticalSyncEnabled( true );

	// GUI.
	ImGui::SFML::Init(window);

	bool updatePreview = true;
	std::string spsLabel;
	std::array<char, 255> scenePath = {"data/scene.json"};

	// Load scene.
	Scene scene;
	scene.loadFromJSON( std::string( scenePath.data() ) );

	// Set up RenderManager.
	RenderManager rm( scene, numThreads );
	rm.setUpdateImage( true );

	// Set up sprite to display.
	sf::Image img;
	img.create( 1, 1, sf::Color::Black );

	sf::Texture tex;
	tex.loadFromImage( img );
	sf::Sprite sprite( tex );

	// Clock for measuring the frame time
	sf::Clock frameClock;
	sf::Clock renderPassClock;

	unsigned int prevSamples = 0;


	// Main loop.
	while( window.isOpen() ){
		auto frameTime = frameClock.restart();

		// Event loop.
		sf::Event event;
		while( window.pollEvent( event ) ){
			ImGui::SFML::ProcessEvent(event);

			if( event.type == sf::Event::Closed ){
				window.close();
			}
			else if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F2 ){
				img.saveToFile(
					"corpuscle-" + std::to_string(renderSize.x) + "x" + std::to_string(renderSize.y) +
					"-" + std::to_string( rm.getSamples() ) + "SPP.png"
				);
			}
		}

		ImGui::SFML::Update(window, frameTime);

		unsigned int samples = rm.getSamples();

		if( prevSamples != samples ){
			int sps = static_cast<int>( std::floor( renderSize.x * renderSize.y / renderPassClock.restart().asSeconds() ) );
			prevSamples = samples;

			if( updatePreview ){
				// Update Image.

				img = rm.getImage();
				tex.loadFromImage( img );

				// Update sprite in case the image dimension changed.
				sprite.setTexture( tex, true );

				renderSize = sf::Vector2u( rm.getResolution().x, rm.getResolution().y );

				float scale( std::min( static_cast<float>( windowSize.x ) / static_cast<float>( renderSize.x ),
									   static_cast<float>( windowSize.y ) / static_cast<float>( renderSize.y ) ) );
				sprite.setScale( scale, scale );
				sprite.setPosition( std::floor( ( windowSize.x - sprite.getGlobalBounds().width )  / 2.f ),
									std::floor( ( windowSize.y - sprite.getGlobalBounds().height ) / 2.f ) );
			}

			spsLabel = "SPS: " + std::to_string( sps );

			window.setTitle(
				"Corpuscle @ "+std::to_string(renderSize.x) + "x" + std::to_string(renderSize.y) +
					" & " + std::to_string(numThreads) + " Threads - SPP: " + std::to_string( samples )
			);
		}

		ImGui::SetNextWindowSize( {guiSize, window.getSize().y} );
		ImGui::SetNextWindowPos( {window.getSize().x - guiSize, 0} );

		ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);


		if (ImGui::Button("Start")) {
			rm.startRendering();
		}
		if (ImGui::Button("Pause")) {
			rm.stopRendering();
		}

		ImGui::InputText( "##scenePath", scenePath.data(), scenePath.size());
		if (ImGui::Button("Load")) {
			rm.stopRendering();

			if( !scene.loadFromJSON( std::string( scenePath.data() ) ) ){
				window.close();
			}
			prevSamples = 0;
			rm.reset();
		}

		ImGui::Checkbox( "Update preview", &updatePreview );
		rm.setUpdateImage( updatePreview );

		ImGui::LabelText( "##sps", spsLabel.c_str() );

		ImGui::End();


		// Drawing.
		window.resetGLStates(); // I'm not sure why this is needed.
		window.clear();

		window.draw( sprite );
		ImGui::Render();

		window.display();
	}

	ImGui::SFML::Shutdown();
}
