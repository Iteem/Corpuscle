#include <iostream>

#include <glm/glm.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFML/Graphics.hpp>

#include "renderer/rendermanager.hpp"


const unsigned int guiSize = 250;

const int sampleDisplayInterval = 10;
const unsigned int numThreads = 4;

int main()
{
	sf::Vector2u windowSize( 1280, 720 );
	sf::Vector2u renderSize( windowSize );

	// Window.
	sf::RenderWindow window( sf::VideoMode(windowSize.x + guiSize, windowSize.y), "Corpuscle", sf::Style::Titlebar | sf::Style::Close );
	window.setVerticalSyncEnabled( true );

	// GUI.
	sfg::SFGUI sfgui;

	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.f );

	auto checkButton = sfg::CheckButton::Create( "Update preview" );
	checkButton->SetActive( true );
	box->Pack( checkButton );

	auto spsLabel = sfg::Label::Create( "SPS: 0" );
	box->Pack( spsLabel );

	auto alignment = sfg::Alignment::Create();
	alignment->SetScale( sf::Vector2f( 0.f, 0.f ) );
	alignment->SetAlignment( sf::Vector2f( 0.5f, 0.5f ) );
	alignment->Add( box );

	auto sfgWindow = sfg::Window::Create( sfg::Window::Style::BACKGROUND );
	sfgWindow->SetPosition( sf::Vector2f( windowSize.x, 0.f ) );
	sfgWindow->SetRequisition( sf::Vector2f( guiSize, windowSize.y ) );
	sfgWindow->Add( alignment );

	sfg::Desktop desktop;
	desktop.Add( sfgWindow );

	// Load scene.
	Scene scene;
	scene.loadFromJSON( "data/scene.json" );

	// Set up RenderManager and start the rendering process.
	RenderManager rm( scene, numThreads );
	rm.setUpdateImage( true );
	rm.startRendering();

	renderSize = sf::Vector2u( rm.getResolution().x, rm.getResolution().y );

	// Set up sprite to display.
	sf::Image img;
	img.create( renderSize.x, renderSize.y, sf::Color::Black );

	sf::Texture tex;
	tex.loadFromImage( img );
	sf::Sprite sprite( tex );
	float scale( std::min( static_cast<float>( windowSize.x ) / static_cast<float>( renderSize.x ),
	                       static_cast<float>( windowSize.y ) / static_cast<float>( renderSize.y ) ) );
	sprite.scale( scale, scale );
	sprite.setPosition( std::floor( ( windowSize.x - sprite.getGlobalBounds().width )  / 2.f ),
	                    std::floor( ( windowSize.y - sprite.getGlobalBounds().height ) / 2.f ) );

	checkButton->GetSignal( sfg::CheckButton::OnStateChange ).Connect( [&]{ rm.setUpdateImage( checkButton->IsActive() ); } );

	// Clock for measuring the frame time
	sf::Clock frameClock;
	sf::Clock renderPassClock;

	unsigned int prevSamples = 0;

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
			int sps = std::floor( renderSize.x * renderSize.y / renderPassClock.restart().asSeconds() );
			prevSamples = samples;

			if( checkButton->IsActive() ){
				// Update Image.
				img = rm.getImage();
				tex.loadFromImage( img );
			}

			spsLabel->SetText( "SPS: " + std::to_string( sps ) );

			window.setTitle(
				"Corpuscle @ "+std::to_string(renderSize.x) + "x" + std::to_string(renderSize.y) +
					" & " + std::to_string(numThreads) + " Threads - SPP: " + std::to_string( samples )
			);
		}


		// Drawing.
		window.clear();

		window.draw( sprite );
		sfgui.Display( window );

		window.display();
	}
}
