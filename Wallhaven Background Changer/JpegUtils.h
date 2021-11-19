/// <summary>
/// Whaven Wallpaper
/// WHaven Wallpaper is a desktop wallpaper change program for change your wallpapers directly from [wallhaven](https://wallhaven.cc/).
///	Copyright(C) 2021 EndChapter
///
///	This program is free software : you can redistribute itand /or modify
///	it under the terms of the GNU General Public License as published by
///	the Free Software Foundation, either version 3 of the License, or
///	(at your option) any later version.
///
///	This program is distributed in the hope that it will be useful,
///	but WITHOUT ANY WARRANTY; without even the implied warranty of
///	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
/// GNU General Public License for more details.
///
///	You should have received a copy of the GNU General Public License
///	along with this program.If not, see < https://www.gnu.org/licenses/>.
/// </summary>
#pragma once

// Create global callback delegates for handling
public delegate System::Void jpegDownloadCallback(int statuscode);
public delegate System::Void setWallpaperCallback(int statuscode);

ref struct JpegUtils
{
	// Default constructor for downlading jpeg
	public: JpegUtils(String^ urlPath, String^ id, String^ type, String^ _purity, bool _download, jpegDownloadCallback^ jdc) {
		_url = urlPath;
		fileID = id;
		fileType = type;
		jDownCb = jdc;
		purity = _purity;
		download = _download;
	}

	// Default constructor for setting wallpaper
	public:	JpegUtils(String^ id, String^ type, String^ _purity, bool _download, setWallpaperCallback^ sWCBack) {
		fileID = id;
		fileType = type;
		sWCB = sWCBack;
		purity = _purity;
		download = _download;
	}
	// True if user selected to download images.
	public: bool download;

	// Url for image
	public: String^ _url;

	// Wallhaven id for changing filename
	public: String^ fileID;

	// Filetype: png or jpeg
	public: String^ fileType;

	// Purity for foldering
	public: String^ purity;

	// Callbacks: I explained Callbacks in MyForm1.h and I don't explain here again.
	public: jpegDownloadCallback^ jDownCb;
	public: setWallpaperCallback^ sWCB;
	
	//
	// Function: download_jpeg: this function downloads specified jpeg.
	// 
	// returns void.
	//
	public: System::Void download_jpeg()
	{
		// Default directory is out/(specified purity)
		String^ directory = "out\\" + purity;

		// Default filename is wallhaven-(filename).(filetype)
		String^ out = gcnew String("\\wallhaven-") + fileID + gcnew String(".") + fileType;

		// Create WebClient instance for download jpeg
		if (System::Net::WebClient^ wb = gcnew System::Net::WebClient()) {
			// If user selects not save images.
			if (!download)
			{
				// then directory is just /out not /out/(specified purity)
				directory = "out";

				// and filename is just out.jpg or out.png
				out = "\\out." + fileType;

				// Create directory if not
				System::IO::Directory::CreateDirectory(directory);

				// Download url(Specified in default constructor) in /out/out.png or /out/out.jpg 
				wb->DownloadFile(_url, directory + out);

				// Handle Callback
				jDownCb(0);

				// Finish Function
				return;
			}
			// If user selects to save images.
			else {
				// Then go on with defaults and create specified directory if not
				System::IO::Directory::CreateDirectory(directory);

				// If specified file is not downladed already.
				if (!System::IO::File::Exists(directory + out))
				{
					// Then go on download
					wb->DownloadFile(_url, directory + out);

					// Handle Callback and return
					jDownCb(0);
					return;
				}
				// If specified file is downladed already.
				else
				{
					// Do nothing, file is already downloaded you can handle callback and return
					jDownCb(0);
					return;
				}
			}
		}


	};

	//
	// Function: setWallpaper: simply sets background image in specified file.
	// 
	// returns void.
	// 
	// Warning: You will call with delegate if you want to use this function in events or threads.
	//
	public: delegate System::Void _setWallpaper();
	public: System::Void setWallpaper() {
		// Create path variable for get path
		wchar_t path[_MAX_PATH];

		// Create wchar_t* for casting strings.
		pin_ptr<const wchar_t> wch;

		// If user selects to save images.
		if(download)
			// Get file name and purity for get path
			wch = PtrToStringChars(gcnew String("out\\") + purity + gcnew String("\\wallhaven-") + fileID + gcnew String(".") + fileType);
		// If user selects to not save images.
		else
			// Then filename is /out/out.jpg or /out/out.png
			wch = PtrToStringChars(gcnew String("out\\out.") + fileType);

		// Get full path and write it in path variable
		wchar_t* rtVal = _wfullpath(path, wch, _MAX_PATH);

		// Set path to wallpaper
		if (SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, path, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE))
		{
			// If ok then ok call callback handler and return
			sWCB(0);
			return;
		}
		else
		{
			// If not then  call callback handler, there is a error and status code is 12
			sWCB(12);
			return;
		}
	}

};