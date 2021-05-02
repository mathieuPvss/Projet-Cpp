#include "Game.h"

//fonction privé 
void Game::initVariables()
{
	this->window = nullptr;

	//Game 
	this->endGame = false;
	this->points = 0;
	this->vie = 20;
	this->enemySpawnTimerMax = 20.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;
}

void Game::initWindow()
{
	this->videoMode.height = 600; //taille de la fenetre 
	this->videoMode.width = 800;//==

	this->window = new sf::RenderWindow(this->videoMode, "Point and Click", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60); //limiter les fps
}


void Game::initText() //gerer le font 
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");
}

void Game::initEnemies() //initiation des carrés/ennemi
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setFillColor(sf::Color::Cyan);
}

//constructeur
Game::Game()
{ //appel des fonctions dans le constructeur 
	this->initVariables();
	this->initWindow();
	this->initText();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

//return si la fenetre est ouverte 
const bool Game::running() const 
{
	return this->window->isOpen();
}

//return si la partie est fini ou pas
const bool Game::getEndGame() const  
{
	return this->endGame;
}

//Fonctions
void Game::spawnEnemy()
{
	
	this->enemy.setPosition( //position aleatoires des carrés
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	
	int type = rand() % 5;

	//selon la position des carrés un type de carré est attribuer 
	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(10.f, 10.f)); //size du carré
		this->enemy.setFillColor(sf::Color::Magenta); //couleur du carré
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	}

	//apparait l'ennemi
	this->enemies.push_back(this->enemy);
}

//gestion des events (click ,clavier ect...)
void Game::pollEvents()
{
	//Event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed: //event quand on clique sur la croix
			this->window->close();
			break;
		case sf::Event::KeyPressed: //quand on appui sur le clavier 
			if (this->ev.key.code == sf::Keyboard::Space) //quand on appui sur la barre d'espace ca ferme le jeu
				this->window->close();
			break;
		}
	}
}

void Game::updateMousePositions() //recuperer la position de la souri 
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText() //affichage du score et de la vie 
{
	std::stringstream ss;

	ss << "Points: " << this->points << "\n"
		<< "vie: " << this->vie << "\n";

	this->uiText.setString(ss.str());
}

void Game::updateEnemies()
{
	if (this->enemies.size() < this->maxEnemies)
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->spawnEnemy();//appel de la fonction spawn
			this->enemySpawnTimer = 0.f; //reset du timer 
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	//deplacement du carré
	for (int i = 0; i < this->enemies.size(); i++)
	{
		bool deleted = false;

		this->enemies[i].move(0.f, 5.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->vie -= 1;
			std::cout << "Vie: " << this->vie << "\n";
		}
	}

	//condition si on clique bien sur le carré
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					//attribution du score selon la taille(type) du carré
					if (this->enemies[i].getFillColor() == sf::Color::Magenta)
						this->points += 10;
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						this->points += 7;
					else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
						this->points += 5;
					else if (this->enemies[i].getFillColor() == sf::Color::Red)
						this->points += 3;
					else if (this->enemies[i].getFillColor() == sf::Color::Green)
						this->points += 1;

					std::cout << "Points: " << this->points << "\n";

					//supprime le carré
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
		}
	}
	else
	{
		this->mouseHeld = false;
	}
}

void Game::update()
{
	this->pollEvents();

	if (this->endGame == false)
	{
		this->updateMousePositions();

		this->updateText();

		this->updateEnemies();
	}

	//condition de fin de jeu
	if (this->vie <= 0)
		this->endGame = true;
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::render()
{
	

	this->window->clear();

	//dessine le contenu sur la fenetre
	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	this->window->display();
}