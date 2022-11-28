#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <cstdint>

uint8_t to_byte(uint32_t num)
{
	if (num < 0)
	{
		return 0;
	}
	else if (num > 255)
	{
		return 255;
	}
	else
	{
		return (uint8_t)num;
	}
}

uint32_t brightness_change(uint32_t color, int8_t brightness)
{
	uint8_t r = to_byte(((color & 0xFF000000) >> 24) + brightness);
	uint8_t g = to_byte(((color & 0x00FF0000) >> 16) + brightness);
	uint8_t b = to_byte(((color & 0x0000FF00) >> 8) + brightness);
	
	uint32_t x = r << 24 | g << 16 | b << 8 | color & 0xFF;

	return x; 
}

int main(int argc, char **argv)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	std::ifstream fin(argv[1], std::ios::binary);
	std::ofstream fout(argv[2], std::ios::binary);

	int brightness = 0;

	if (argv[3])
	{
		brightness = atoi(argv[3]);
	}

	if (!fin.is_open())
	{
		std::cerr << "Can`t open file: { " << argv[1] << " }\n";
		return -1;
	}

	uint16_t width;
	uint16_t height;
	uint8_t bit;
	uint16_t palet_num;

	fin.read((char*)&width, sizeof(uint16_t));
	fin.read((char*)&height, sizeof(uint16_t));
	fin.read((char*)&bit, sizeof(uint8_t));
	fin.read((char*)&palet_num, sizeof(uint16_t));
	
	uint32_t* palet  = new uint32_t[palet_num];
	uint8_t*  pixels = new uint8_t[width * height];
	uint8_t*	pixels_ptr = pixels;
	uint32_t* result = new uint32_t[width * height * sizeof(uint32_t)];

	fin.read((char*)palet, palet_num * sizeof(uint32_t));
	fin.read((char*)pixels, width * height);

	uint32_t w_idx;
	uint32_t h_idx;

	sf::Image image;


	for (uint32_t i = 0; i < height; i++)
	{
		for (uint32_t j = 0; j < width; j++)
		{
			if (j % 2 == 0)
			{
				w_idx = (*pixels_ptr >> 6) & 0b11;
				h_idx = (*pixels_ptr >> 4) & 0b11;
			}
			else
			{
				w_idx = (*pixels_ptr >> 2) & 0b11;
				h_idx = *pixels_ptr & 0b11;

				pixels_ptr++;
			} 
			
			result[j + i * height] = brightness_change(palet[w_idx + h_idx * 4], brightness);
		}
	}

	image.create((unsigned int)width, (unsigned int)height, (uint8_t*)result);

	sf::Texture texture;

	texture.loadFromImage(image);

	sf::Sprite sprite(texture);

	sf::RenderWindow window(sf::VideoMode(width, height), "Lab2", sf::Style::Default, settings);

	if (fout.is_open())
	{
		fout.write((char*)&width, sizeof(uint16_t));
		fout.write((char*)&height, sizeof(uint16_t));
		fout.write((char*)&bit, sizeof(uint8_t));
		fout.write((char*)&palet_num, sizeof(uint16_t));
		fout.write((char*)palet, palet_num * sizeof(uint32_t));
		fout.write((char*)result, width * height * sizeof(uint32_t));
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.draw(sprite);

		window.display();
	}

	return 0;
}
