#include <iostream>
#include <vector>
#include <sstream>
#include <bitset>
#include "SFML\Graphics\Image.hpp"

void encode(sf::Image&, std::string&);
void decode(sf::Image&);

int main(int argc, char *argv[])
{
	std::cout << "Secret Image - v1 - W.J.Pearson 2018" << std::endl;
	std::cout << "Loading Image ";

	if(argc == 1)
	{
		std::cout << "no images found!" << std::endl;
		std::cout << "Drag image onto the .exe to edit them." << std::endl;
		std::cin.ignore();
		return 0;
	}

	std::cout << argv[1] << std::endl;
	sf::Image image;
	image.loadFromFile(argv[1]);

	bool okay = false;
	while(!okay)
	{
		std::cout << "Encode or Decode: ";
		std::string dowhat;
		std::cin >> dowhat;
		if(dowhat == "E" || dowhat == "e" || dowhat == "Encode" || dowhat == "encode")
		{
			std::string img_path = argv[1];
			encode(image, img_path);
			okay = true;
			std::cout << "Complete!" << std::endl;
		}
		else if(dowhat == "D" || dowhat == "d" || dowhat == "Decode" || dowhat == "decode")
		{
			decode(image);
			okay = true;
			std::cout << "Complete!" << std::endl;
		}
		else if(dowhat == "Q" || dowhat == "q" || dowhat == "Quit" || dowhat == "quit")
		{
			okay = true;
		}
		else
		{
			std::cout << "Invalid Choice" << std::endl;
		}
	}

	std::cout << "Press ENTER to exit" << std::endl;
	std::cin.ignore();

	return 0;
}

void encode(sf::Image &image, std::string &save_file)
{
	std::cin.ignore();
	std::cout << "Enter message: ";
	std::string message;
	std::getline(std::cin, message);

	std::stringstream binary;
	for(int i = 0; i < message.size(); i++)
	{
		binary << std::bitset<8>(message[i]);
	}
	int bitlen = binary.str().size();
	int bitpos = 0;

	for(int i = 0; i < image.getSize().x; i++)
	{
		for(int j = 0; j < image.getSize().y; j++)
		{
			int red = image.getPixel(i, j).r;
			int gre = image.getPixel(i, j).g;
			int blu = image.getPixel(i, j).b;
			blu = ((blu / 16) * 16) + 8;
			if(binary.str()[bitpos] == '1')
			{
				blu++;
			}
			int alp = image.getPixel(i, j).a;
			image.setPixel(i, j, sf::Color(red, gre, blu, alp));

			bitpos++;
			if(bitpos == bitlen)
			{
				std::cout << "End thing" << std::endl;
				if(j < image.getSize().y - 1)
				{
					j++;
					red = image.getPixel(i, j).r;
					gre = image.getPixel(i, j).g;
					blu = image.getPixel(i, j).b;
					blu = (blu / 16) * 16;
					blu += 2;
					alp = image.getPixel(i, j).a;
				}
				else if(i < image.getSize().x - 1)
				{
					i++;
					j=0;
					red = image.getPixel(i, j).r;
					gre = image.getPixel(i, j).g;
					blu = image.getPixel(i, j).b;
					blu = (blu / 16) * 16;
					blu += 2;
					alp = image.getPixel(i, j).a;
				}
				image.setPixel(i, j, sf::Color(red, gre, blu, alp));
				break;
			}
		}
		if(bitpos == bitlen)
		{
			break;
		}
	}

	std::cout << "Encode Complete!" << std::endl;
	if(bitpos < bitlen)
	{
		std::cout << "Message Too Long!" << std::endl;
	}

	//Curate file path and extension
	size_t end = save_file.find_last_of(".");
	std::string file = save_file.substr(0, end);
	std::string extn = save_file.substr(end);

	file += "_secrets";
	file += extn;

	std::cout << "Saving to: " << file << std::endl;
	image.saveToFile(file);

	return;
}

void decode(sf::Image &image)
{
	std::cin.ignore();
	std::stringstream binary;
	int rem;
	for(int i = 0; i < image.getSize().x; i++)
	{
		for(int j = 0; j < image.getSize().y; j++)
		{
			int blu = image.getPixel(i, j).b;
			rem	= (blu % 16) - 8;
			if(rem == 2)
			{
				break;
			}
			else
			{
				binary << rem;
			}
		}
		if(rem == 2)
		{
			break;
		}
	}
	
	std::string message;
	while(binary.good())
	{
		std::bitset<8> bits;
		binary >> bits;
		char c = char(bits.to_ulong());
		message += c;
	}

	std::cout << std::endl << "Message:" << std::endl;
	std::cout << message << std::endl << std::endl;
	return;
}