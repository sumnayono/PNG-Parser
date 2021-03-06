#include "PNG/Utils.hpp"

/* Unsigned */

namespace PNG
{
	std::uint16_t Read(std::array<std::uint8_t, 2> data)
	{
		std::uint16_t i = 0;
		i |= (0xFF00 & static_cast<std::uint16_t>(data[0] << 8));
		i |= (0x00FF & data[1]);
		return i;
	}

	std::uint32_t Read(std::array<std::uint8_t, 4> data)
	{
		std::uint32_t i = 0;
		i |= (0xFF000000 & (static_cast<std::uint32_t>(data[0]) << 24));
		i |= (0x00FF0000 & (static_cast<std::uint32_t>(data[1]) << 16));
		i |= (0x0000FF00 & (static_cast<std::uint32_t>(data[2]) << 8));
		i |= (0x000000FF & data[3]);
		return i;
	}

	std::uint64_t Read(std::array<std::uint8_t, 8> data)
	{
		std::uint64_t i = 0;
		i |= (0xFF00000000000000 & (static_cast<std::uint64_t>(data[0]) << 56));
		i |= (0x00FF000000000000 & (static_cast<std::uint64_t>(data[1]) << 48));
		i |= (0x0000FF0000000000 & (static_cast<std::uint64_t>(data[2]) << 40));
		i |= (0x000000FF00000000 & (static_cast<std::uint64_t>(data[3]) << 32));
		i |= (0x00000000FF000000 & (static_cast<std::uint64_t>(data[4]) << 24));
		i |= (0x0000000000FF0000 & (static_cast<std::uint64_t>(data[5]) << 16));
		i |= (0x000000000000FF00 & (static_cast<std::uint64_t>(data[6]) << 8));
		i |= (0x00000000000000FF & data[7]);
		return i;
	}

	std::istream& Read(std::istream& is, std::uint8_t& i)
	{
		i = is.get();
		return is;
	}

	std::istream& Read(std::istream& is, std::uint16_t& i, Endian endian)
	{
		std::array<std::uint8_t, 2> data{};
		if (endian == Endian::LittleEndian)
		{
			for (auto it = data.rbegin(); it != data.rend(); ++it)
				*it = is.get();
		}
		else if (endian == Endian::BigEndian)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
				*it = is.get();
		}


		i = Read(data);

		return is;
	}

	std::istream& Read(std::istream& is, std::uint32_t& i, Endian endian)
	{
		i = 0;
		std::array<std::uint8_t, 4> data{};

		if (endian == Endian::LittleEndian)
		{
			for (auto it = data.rbegin(); it != data.rend(); ++it)
				*it = is.get();
		}
		else if (endian == Endian::BigEndian)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
				*it = is.get();
		}

		i = Read(data);

		return is;
	}

	std::istream& Read(std::istream& is, std::uint64_t& i, Endian endian)
	{
		i = 0;
		std::array<std::uint8_t, 8> data{};
		if (endian == Endian::LittleEndian)
		{
			for (auto it = data.rbegin(); it != data.rend(); ++it)
				*it = is.get();
		}
		else if (endian == Endian::BigEndian)
		{
			for (auto it = data.begin(); it != data.end(); ++it)
				*it = is.get();
		}

		i = Read(data);

		return is;
	}

	std::vector<std::uint8_t> Inflate(std::vector<std::uint8_t> deflatedData, size_t readBufferSize)
	{

		std::vector<std::uint8_t> inflated(readBufferSize);

		z_stream infStream;
		infStream.zalloc = Z_NULL;
		infStream.zfree  = Z_NULL;
		infStream.opaque = Z_NULL;
		
		infStream.avail_in = static_cast<uInt>(deflatedData.size());
		infStream.next_in  = reinterpret_cast<Bytef*>(deflatedData.data());
		infStream.avail_out = static_cast<uInt>(inflated.size());
		infStream.next_out = reinterpret_cast<Bytef*>(inflated.data());

		inflateInit(&infStream);
		inflate(&infStream, Z_NO_FLUSH);
		inflateEnd(&infStream);

		return inflated;
	}
}