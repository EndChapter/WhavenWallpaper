#pragma once
public delegate System::Void jpegDownloadCallback(int statuscode);
public delegate System::Void setWallpaperCallback(int statuscode);

ref struct JpegUtils
{
	public:
		bool download;
		String^ _url;
		String^ fileID;
		String^ fileType;
		String^ purity;
		jpegDownloadCallback^ jDownCb;
		setWallpaperCallback^ sWCB;
		JpegUtils(String^ urlPath, String^ id, String^ type, String^ _purity, bool _download, jpegDownloadCallback^ jdc) {
			_url = urlPath;
			fileID = id;
			fileType = type;
			jDownCb = jdc;
			purity = _purity;
			download = _download;
		}
		JpegUtils(String^ id, String^ type, String^ _purity, bool _download, setWallpaperCallback^ sWCBack) {
			fileID = id;
			fileType = type;
			sWCB = sWCBack;
			purity = _purity;
			download = _download;
		}

	public: System::Void download_jpeg()
	{
		String^ directory = "out\\" + purity;
		String^ out = gcnew String("\\wallhaven-") + fileID + gcnew String(".") + fileType;
		System::Net::WebClient^ wb = gcnew System::Net::WebClient();
		if (!download)
		{
			directory = "out";
			out = "\\out." + fileType;
			System::IO::Directory::CreateDirectory(directory);
			wb->DownloadFile(_url, directory + out);
			jDownCb(0);
			return;
		}
		else {
			System::IO::Directory::CreateDirectory(directory);
			if (!System::IO::File::Exists(directory + out))
			{
				wb->DownloadFile(_url, directory + out);
				// std::rename("out1.jpg", "out.jpg");
				jDownCb(0);
			}
			else
			{
				jDownCb(0);
			}
		}
	};
	public: delegate System::Void _setWallpaper();
	public: System::Void setWallpaper() {
		wchar_t path[_MAX_PATH];
		pin_ptr<const wchar_t> wch;
		if(download)
			wch = PtrToStringChars(gcnew String("out\\") + purity + gcnew String("\\wallhaven-") + fileID + gcnew String(".") + fileType);
		else
			wch = PtrToStringChars(gcnew String("out\\out.") + fileType);
		wchar_t* rtVal = _wfullpath(path, wch, _MAX_PATH);
		if (SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, path, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE))
		{
			sWCB(0);
		}
		else
		{
			sWCB(12);
		}
	}

};