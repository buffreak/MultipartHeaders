#include "MimeType.cpp"
#include<string>
#include<sstream>
#include<fstream>
#include<random>
#include<filesystem>

class Multipart {

	const std::string BOUNDARY_DASH = "-----------------------------";
	std::size_t randomID{};
	std::fstream fileStream;
	std::string contentTransferEncoding, extensionName, getContent;
	std::ostringstream finalHeader;
	std::size_t uniqID{};

protected:
	Multipart& generateUUID() {
		std::random_device rd;
		std::ostringstream buffer;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> distribution(25000000, 250000000);
		this->uniqID = static_cast<std::size_t>(distribution(gen));
		return *this;
	}

protected:
	std::vector<std::string> explode(std::string const& s, char delim){
		std::vector<std::string> results;
		std::istringstream iss(s);

		for (std::string token; std::getline(iss, token, delim);){
			results.push_back(std::move(token));
		}
		return results;
	}

protected:
	 std::string base64_encode(const std::string& in) {

		std::string out;

		int val = 0, valb = -6;
		for (unsigned char c : in) {
			val = (val << 8) + c;
			valb += 8;
			while (valb >= 0) {
				out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
		while (out.size() % 4) out.push_back('=');
		return out;
	}


protected:
	Multipart& encodeData(std::string encodingType = std::string()) {
		std::transform(encodingType.begin(), encodingType.end(), encodingType.begin(), ::tolower);
		if (encodingType == "base64") this->getContent = this->base64_encode(this->getContent);
		return *this;
	}

protected:
	Multipart& getFileContent(std::string& filename, const std::string& encoding = std::string()) {
		std::ostringstream container;
		this->fileStream.open(std::filesystem::current_path().string() + "\\" + filename, std::ifstream::binary | std::ios_base::in | std::ios_base::out);
		if (this->fileStream.is_open()) {
			container << this->fileStream.rdbuf();
		}
		this->fileStream.close();
		this->getContent = container.str();
		this->encodeData(encoding);
		return *this;
	}

public:
	template<typename _Mime_Data_T, typename _Encoding_Data_T> 
		Multipart& createHeader(_Mime_Data_T& mime, _Encoding_Data_T& contentTransferEncoding = NULL) {
			throw std::exception("This data type does not support yet, only support right now is std::map<std::string, std::string> for mime, and std::vector<std::string> for contentClientEncoding");
			return *this;
	}

	template<> Multipart& createHeader<std::map<std::string, std::string>, std::vector<std::string>>
		(std::map<std::string, std::string>& mime, std::vector<std::string>& contentTransferEncoding)
	{
		this->generateUUID();
		std::size_t key = 0;
		this->finalHeader << "Content-Type: multipart/form-data; boundary=" << Multipart::BOUNDARY_DASH << this->uniqID << "\r\n\n";
		for (auto& iter : mime) {
			this->finalHeader << Multipart::BOUNDARY_DASH << this->uniqID << "\r\n";
			this->finalHeader << "Content-Disposition: form-data; " << "name=\"" << iter.first << "\"";

			std::size_t findExtension = iter.second.find_last_of(".");
			if (findExtension != std::string::npos) {
				this->extensionName = iter.second.substr(findExtension + 1);
				this->finalHeader << "; " << "filename=\"" << iter.second << "\"";
				this->finalHeader << "\r\n" << "Content-Type: " << MimeType::MIME_LIST[this->extensionName];
			}
			else this->finalHeader << ";";

			if (findExtension != std::string::npos) {
				this->getFileContent(iter.second, !contentTransferEncoding.empty() && !contentTransferEncoding[key].empty() ? contentTransferEncoding[key] : std::string());
				if (!contentTransferEncoding.empty() && !contentTransferEncoding[key].empty()) this->finalHeader << "\r\n" << "Content-Transfer-Encoding: " << contentTransferEncoding[key];
			}
			
			this->finalHeader << "\r\n\n";

			if (findExtension != std::string::npos) {
				this->finalHeader << this->getContent << "\r\n";
			}
			else this->finalHeader << iter.second << "\r\n";

			key++;
		}
		this->finalHeader << Multipart::BOUNDARY_DASH << this->uniqID << "--";
		return *this;
	}

	Multipart& createHeader(std::map<std::string, std::string> mime, std::vector<std::string>&& contentTransferEncoding) {
		this->createHeader(mime, contentTransferEncoding);
		return *this;
	}

public:
	std::string create() {
		return this->finalHeader.str();
	}

};