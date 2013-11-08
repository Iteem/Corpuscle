#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window( sf::VideoMode(800, 600), "Corpuscle" );
	window.setVerticalSyncEnabled( true );

	// main loop
	while( window.isOpen() ){
		// event loop
		sf::Event event;
		while( window.pollEvent( event ) ){
			if( event.type == sf::Event::Closed ){
				window.close();
			}
		}

		window.clear();

		window.display();
	}
}
