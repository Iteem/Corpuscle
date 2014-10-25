#include <iostream>

#include <glm/glm.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

#include "renderer/rendermanager.hpp"

const sf::Vector2u  r360p(   640,   360 );
const sf::Vector2u  r480p(   854,   480 );
const sf::Vector2u  r720p(  1280,   720 );
const sf::Vector2u r1080p(  1920,  1080 );
const sf::Vector2u r1440p(  2560,  1440 );
const sf::Vector2u    r4K(  3840,  2160 );
const sf::Vector2u    r8K(  7680,  4320 );
const sf::Vector2u   r16K( 15360,  8640 );

const sf::Vector2u screenSize( r720p );
const sf::Vector2u renderSize( r720p );

const unsigned int guiSize = 80;

const int sampleDisplayInterval = 10;
const unsigned int numThreads = 4;

int main()
{
	// Window.
	sf::RenderWindow window( sf::VideoMode(screenSize.x, screenSize.y), "Corpuscle", sf::Style::Titlebar | sf::Style::Close  );
	window.setVerticalSyncEnabled( true );

	// GUI.
	sfg::SFGUI sfgui;

	auto sfgWindow = sfg::Window::Create( sfg::Window::Style::NO_STYLE );
	sfgWindow->SetPosition( sf::Vector2f( 0.f, screenSize.y - guiSize ) );
	sfgWindow->SetRequisition( sf::Vector2f( screenSize.x, guiSize ) );

	auto box = sfg::Box::Create();
	sfgWindow->Add( box );

	auto alignment = sfg::Alignment::Create();
	alignment->SetScale( sf::Vector2f( 0.f, 0.f ) );
	alignment->SetAlignment( sf::Vector2f( 0.5f, 0.5f ) );
	box->Pack( alignment );

	auto button = sfg::Button::Create( "Close" );
	button->GetSignal( sfg::Widget::OnLeftClick ).Connect( [&window]{ window.close(); } );
	alignment->Add( button );

	sfg::Desktop desktop;
	desktop.Add( sfgWindow );

	// Set up sprite to display.
	sf::Image img;
	img.create( renderSize.x, renderSize.y, sf::Color::Black );

	sf::Texture tex;
	tex.loadFromImage( img );
	sf::Sprite sprite( tex );
	sprite.scale(
		static_cast<float>(screenSize.x)/static_cast<float>(renderSize.x),
		static_cast<float>(screenSize.y)/static_cast<float>(renderSize.y)
	);


	// Set up RenderManager and start the rendering process.
	RenderManager rm( glm::uvec2( renderSize.x, renderSize.y ), numThreads );
	rm.loadSceneFromFile( "data/scene.json" );
	rm.setUpdateImage( false );
	rm.startRendering();

	unsigned int prevSamples = 0;

	// Clock for measuring the frame time
	sf::Clock frameClock;

	// Main loop.
	while( window.isOpen() ){
		unsigned int samples = rm.getSamples();

		float frameTime = frameClock.restart().asSeconds();

		// Event loop.
		sf::Event event;
		while( window.pollEvent( event ) ){
			desktop.HandleEvent( event );

			if( event.type == sf::Event::Closed ){
				window.close();
			}
			else if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F2 ){
				img.saveToFile(
					"corpuscle-"+std::to_string(renderSize.x) + "x" + std::to_string(renderSize.y) +
					"-" + std::to_string( rm.getSamples() ) + "SPP.png"
				);
			}
		}

		desktop.Update( frameTime );

		if( prevSamples != samples ){
			// Only update the image every Nth sample.
			if( samples % sampleDisplayInterval == 0 ){
				prevSamples = samples;

				// Update Image.
				img = rm.getImage();
				tex.loadFromImage( img );

				rm.setUpdateImage( false );
			}
			// Don't forget to request a image update before.
			else if( samples % (sampleDisplayInterval-1) == 0 ){
				rm.setUpdateImage( true );
			}
		}

		window.setTitle(
			"Corpuscle @ "+std::to_string(renderSize.x) + "x" + std::to_string(renderSize.y) +
				" & " + std::to_string(numThreads) + " Threads - SPP: " + std::to_string( samples )
		);

		// Drawing.
		window.clear();

		window.draw( sprite );
		sfgui.Display( window );

		window.display();
	}
}
