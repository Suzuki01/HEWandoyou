#include <bitset>
#include "hidapi.h"
#include "joycon.h"
#include "Windows.h"
#include "common.h"
#include <vector>
#include <chrono>
#include "debug_font.h"
#include <d3dx9.h>



#define CONTROLAR_MAX (3)

float clamp(float a, float min, float max) {
	if (a < min) {
		return min;
	}
	else if (a > max) {
		return max;
	}
	else {
		return a;
	}
}

int16_t uint16_to_int16(uint16_t a) {
	int16_t b;
	char* aPointer = (char*)&a, *bPointer = (char*)&b;
	memcpy(bPointer, aPointer, sizeof(a));
	return b;
}

unsigned char buf[65];
std::vector<Joycon>joycons;
int res = 0;
D3DXVECTOR3 jyro[CONTROLAR_MAX];
D3DXVECTOR3 accel[CONTROLAR_MAX];

//joyconの向きを判定する
int atemp[5];

//JOYSTICK_POSITION_V2 iReport; // The structure that holds the full position data


	Joycon::Joycon(struct hid_device_info *dev) {
		
		deviceNumber = 0; 
		vJoyNumber = 0;// 
		bluetooth = true;
		left_right = 0;// 1: left joycon, 2: right joycon, 3: pro controller
		buttons = 0;
		buttons2 = 0;// for pro controller
		global_count = 0;
		accel.prevX = 0;
		accel.prevY = 0;
		accel.prevZ = 0;
		accel.x = 0;
		accel.y = 0;
		accel.z = 0;


		if (dev->product_id == JOYCON_CHARGING_GRIP) {

			if (dev->interface_number == 0 || dev->interface_number == -1) {
				this->name = std::string("Joy-Con (R)");
				this->left_right = 2;// right joycon
			}
			else if (dev->interface_number == 1) {
				this->name = std::string("Joy-Con (L)");
				this->left_right = 1;// left joycon
			}
		}

		if (dev->product_id == JOYCON_L_BT) {
			this->name = std::string("Joy-Con (L)");
			this->left_right = 1;// left joycon
		}
		else if (dev->product_id == JOYCON_R_BT) {
			this->name = std::string("Joy-Con (R)");
			this->left_right = 2;// right joycon
		}
		else if (dev->product_id == PRO_CONTROLLER) {
			this->name = std::string("Pro Controller");
			this->left_right = 3;// left joycon
		}

		this->serial = _wcsdup(dev->serial_number);

		//printf("Found joycon %c %i: %ls %s\n", L_OR_R(this->left_right), joycons.size(), this->serial, dev->path);
		//printf("Found joycon %c: %ls %s\n", L_OR_R(this->left_right), this->serial, dev->path);
		this->handle = hid_open_path(dev->path);


		if (this->handle == nullptr) {
			printf("Could not open serial %ls: %s\n", this->serial, strerror(errno));
			throw;
		}
	}
	Joycon::~Joycon() {

	}

	void Joycon::hid_exchange(hid_device *handle, unsigned char *buf, int len) {
		if (!handle) return;

		int res;

		res = hid_write(handle, buf, len);

		//if (res < 0) {
		//	printf("Number of bytes written was < 0!\n");
		//} else {
		//	printf("%d bytes written.\n", res);
		//}

		//// set non-blocking:
		//hid_set_nonblocking(handle, 1);

		res = hid_read(handle, buf, 0x40);

		//if (res < 1) {
		//	printf("Number of bytes read was < 1!\n");
		//} else {
		//	printf("%d bytes read.\n", res);
		//}
	}


	void Joycon::send_command(int command, uint8_t *data, int len) {
		unsigned char buf[0x40];
		memset(buf, 0, 0x40);

		if (!bluetooth) {
			buf[0x00] = 0x80;
			buf[0x01] = 0x92;
			buf[0x03] = 0x31;
		}

		buf[bluetooth ? 0x0 : 0x8] = command;
		if (data != nullptr && len != 0) {
			memcpy(buf + (bluetooth ? 0x1 : 0x9), data, len);
		}

		hid_exchange(this->handle, buf, len + (bluetooth ? 0x1 : 0x9));

		if (data) {
			memcpy(data, buf, 0x40);
		}
	}

	void Joycon::send_subcommand(int command, int subcommand, uint8_t *data, int len) {
		unsigned char buf[0x40];
		memset(buf, 0, 0x40);

		uint8_t rumble_base[9] = { (++global_count) & 0xF, 0x00, 0x01, 0x40, 0x40, 0x00, 0x01, 0x40, 0x40 };
		memcpy(buf, rumble_base, 9);

		if (global_count > 0xF) {
			global_count = 0x0;
		}

		// set neutral rumble base only if the command is vibrate (0x01)
		// if set when other commands are set, might cause the command to be misread and not executed
		//if (subcommand == 0x01) {
		//	uint8_t rumble_base[9] = { (++global_count) & 0xF, 0x00, 0x01, 0x40, 0x40, 0x00, 0x01, 0x40, 0x40 };
		//	memcpy(buf + 10, rumble_base, 9);
		//}

		buf[9] = subcommand;
		if (data && len != 0) {
			memcpy(buf + 10, data, len);
		}

		send_command(command, buf, 10 + len);

		if (data) {
			memcpy(data, buf, 0x40); //TODO
		}
	}

	void Joycon::rumble(int frequency, int intensity) {

		unsigned char buf[0x400];
		memset(buf, 0, 0x40);

		// intensity: (0, 8)
		// frequency: (0, 255)

		//	 X	AA	BB	 Y	CC	DD
		//[0 1 x40 x40 0 1 x40 x40] is neutral.


		//for (int j = 0; j <= 8; j++) {
		//	buf[1 + intensity] = 0x1;//(i + j) & 0xFF;
		//}

		buf[1 + 0 + intensity] = 0x1;
		buf[1 + 4 + intensity] = 0x1;

		// Set frequency to increase
		if (this->left_right == 1) {
			buf[1 + 0] = frequency;// (0, 255)
		}
		else {
			buf[1 + 4] = frequency;// (0, 255)
		}

		// set non-blocking:
		hid_set_nonblocking(this->handle, 1);

		send_command(0x10, (uint8_t*)buf, 0x9);
	}

	void Joycon::rumble2(uint16_t hf, uint8_t hfa, uint8_t lf, uint16_t lfa) {
		unsigned char buf[0x400];
		memset(buf, 0, 0x40);


		//int hf		= HF;
		//int hf_amp	= HFA;
		//int lf		= LF;
		//int lf_amp	= LFA;
		// maybe:
		//int hf_band = hf + hf_amp;

		int off = 0;// offset
		if (this->left_right == 2) {
			off = 4;
		}


		// Byte swapping
		buf[0 + off] = hf & 0xFF;
		buf[1 + off] = hfa + ((hf >> 8) & 0xFF); //Add amp + 1st byte of frequency to amplitude byte

												 // Byte swapping
		buf[2 + off] = lf + ((lfa >> 8) & 0xFF); //Add freq + 1st byte of LF amplitude to the frequency byte
		buf[3 + off] = lfa & 0xFF;


		// set non-blocking:
		hid_set_nonblocking(this->handle, 1);

		send_command(0x10, (uint8_t*)buf, 0x9);
	}

	void Joycon::rumble3(float frequency, uint8_t hfa, uint16_t lfa) {

		//Float frequency to hex conversion
		if (frequency < 0.0f) {
			frequency = 0.0f;
		}
		else if (frequency > 1252.0f) {
			frequency = 1252.0f;
		}
		uint8_t encoded_hex_freq = (uint8_t)round(log2((double)frequency / 10.0)*32.0);

		//uint16_t encoded_hex_freq = (uint16_t)floor(-32 * (0.693147f - log(frequency / 5)) / 0.693147f + 0.5f); // old

		//Convert to Joy-Con HF range. Range in big-endian: 0x0004-0x01FC with +0x0004 steps.
		uint16_t hf = (encoded_hex_freq - 0x60) * 4;
		//Convert to Joy-Con LF range. Range: 0x01-0x7F.
		uint8_t lf = encoded_hex_freq - 0x40;

		rumble2(hf, hfa, lf, lfa);
	}
	
	void Joycon::rumble4(float real_LF, float real_HF, uint8_t hfa, uint16_t lfa) {

		real_LF = clamp(real_LF, 40.875885f, 626.286133f);
		real_HF = clamp(real_HF, 81.75177, 1252.572266f);

		////Float frequency to hex conversion
		//if (frequency < 0.0f) {
		//	frequency = 0.0f;
		//} else if (frequency > 1252.0f) {
		//	frequency = 1252.0f;
		//}
		//uint8_t encoded_hex_freq = (uint8_t)round(log2((double)frequency / 10.0)*32.0);

		//uint16_t encoded_hex_freq = (uint16_t)floor(-32 * (0.693147f - log(frequency / 5)) / 0.693147f + 0.5f); // old

		////Convert to Joy-Con HF range. Range in big-endian: 0x0004-0x01FC with +0x0004 steps.
		//uint16_t hf = (encoded_hex_freq - 0x60) * 4;
		////Convert to Joy-Con LF range. Range: 0x01-0x7F.
		//uint8_t lf = encoded_hex_freq - 0x40;



		uint16_t hf = ((uint8_t)round(log2((double)real_HF * 0.01)*32.0) - 0x60) * 4;
		uint8_t lf = (uint8_t)round(log2((double)real_LF * 0.01)*32.0) - 0x40;

		rumble2(hf, hfa, lf, lfa);
	}
	

	void Joycon::rumble_freq(uint16_t hf, uint8_t hfa, uint8_t lf, uint16_t lfa) {
		unsigned char buf[0x400];
		memset(buf, 0, 0x40);


		//int hf		= HF;
		//int hf_amp	= HFA;
		//int lf		= LF;
		//int lf_amp	= LFA;
		// maybe:
		//int hf_band = hf + hf_amp;

		int off = 0;// offset
		if (this->left_right == 2) {
			off = 4;
		}


		// Byte swapping
		buf[0 + off] = hf & 0xFF;
		buf[1 + off] = hfa + ((hf >> 8) & 0xFF); //Add amp + 1st byte of frequency to amplitude byte

												 // Byte swapping
		buf[2 + off] = lf + ((lfa >> 8) & 0xFF); //Add freq + 1st byte of LF amplitude to the frequency byte
		buf[3 + off] = lfa & 0xFF;


		// set non-blocking:
		hid_set_nonblocking(this->handle, 1);

		send_command(0x10, (uint8_t*)buf, 0x9);
	}


	void Joycon::setGyroOffsets() {
		float thresh = 0.1;
		if (abs(this->gyro.roll) > thresh || abs(this->gyro.pitch) > thresh || abs(this->gyro.yaw) > thresh) {
			return;
		}

		//average = current + ((newData - current) / n);
		this->gyro.offset.n += 1;
		this->gyro.offset.roll = this->gyro.offset.roll + ((this->gyro.roll - this->gyro.offset.roll) / this->gyro.offset.n);
		this->gyro.offset.pitch = this->gyro.offset.pitch + ((this->gyro.pitch - this->gyro.offset.pitch) / this->gyro.offset.n);
		this->gyro.offset.yaw = this->gyro.offset.yaw + ((this->gyro.yaw - this->gyro.offset.yaw) / this->gyro.offset.n);
		//this->gyro.offset.roll	= this->gyro.roll;
		//this->gyro.offset.pitch = this->gyro.pitch;
		//this->gyro.offset.yaw	= this->gyro.yaw;
	}

	
	int Joycon::init_bt() {

		this->bluetooth = true;

		unsigned char buf[0x40];
		memset(buf, 0, 0x40);


		// set blocking to ensure command is recieved:
		hid_set_nonblocking(this->handle, 0);

		// Enable vibration
		printf("Enabling vibration...\n");
		buf[0] = 0x01; // Enabled
		send_subcommand(0x1, 0x48, buf, 1);

		// Enable IMU data
		printf("Enabling IMU data...\n");
		buf[0] = 0x01; // Enabled
		send_subcommand(0x01, 0x40, buf, 1);


		// Set input report mode (to push at 60hz)
		// x00	Active polling mode for IR camera data. Answers with more than 300 bytes ID 31 packet
		// x01	Active polling mode
		// x02	Active polling mode for IR camera data.Special IR mode or before configuring it ?
		// x21	Unknown.An input report with this ID has pairing or mcu data or serial flash data or device info
		// x23	MCU update input report ?
		// 30	NPad standard mode. Pushes current state @60Hz. Default in SDK if arg is not in the list
		// 31	NFC mode. Pushes large packets @60Hz
		printf("Set input report mode to 0x30...\n");
		buf[0] = 0x30;
		send_subcommand(0x01, 0x03, buf, 1);

		// @CTCaer

		// get calibration data:
		printf("Getting calibration data...\n");
		memset(factory_stick_cal, 0, 0x12);
		memset(user_stick_cal, 0, 0x16);
		memset(sensor_model, 0, 0x6);
		memset(stick_model, 0, 0x12);
		memset(factory_sensor_cal, 0, 0x18);
		memset(user_sensor_cal, 0, 0x1A);
		memset(factory_sensor_cal_calm, 0, 0xC);
		memset(user_sensor_cal_calm, 0, 0xC);
		memset(sensor_cal, 0, sizeof(sensor_cal));
		memset(stick_cal_x_l, 0, sizeof(stick_cal_x_l));
		memset(stick_cal_y_l, 0, sizeof(stick_cal_y_l));
		memset(stick_cal_x_r, 0, sizeof(stick_cal_x_r));
		memset(stick_cal_y_r, 0, sizeof(stick_cal_y_r));


		get_spi_data(0x6020, 0x18, factory_sensor_cal);
		get_spi_data(0x603D, 0x12, factory_stick_cal);
		get_spi_data(0x6080, 0x6, sensor_model);
		get_spi_data(0x6086, 0x12, stick_model);
		get_spi_data(0x6098, 0x12, &stick_model[0x12]);
		get_spi_data(0x8010, 0x16, user_stick_cal);
		get_spi_data(0x8026, 0x1A, user_sensor_cal);


		// get stick calibration data:

		// factory calibration:

		if (this->left_right == 1 || this->left_right == 3) {
			stick_cal_x_l[1] = (factory_stick_cal[4] << 8) & 0xF00 | factory_stick_cal[3];
			stick_cal_y_l[1] = (factory_stick_cal[5] << 4) | (factory_stick_cal[4] >> 4);
			stick_cal_x_l[0] = stick_cal_x_l[1] - ((factory_stick_cal[7] << 8) & 0xF00 | factory_stick_cal[6]);
			stick_cal_y_l[0] = stick_cal_y_l[1] - ((factory_stick_cal[8] << 4) | (factory_stick_cal[7] >> 4));
			stick_cal_x_l[2] = stick_cal_x_l[1] + ((factory_stick_cal[1] << 8) & 0xF00 | factory_stick_cal[0]);
			stick_cal_y_l[2] = stick_cal_y_l[1] + ((factory_stick_cal[2] << 4) | (factory_stick_cal[2] >> 4));

		}

		if (this->left_right == 2 || this->left_right == 3) {
			stick_cal_x_r[1] = (factory_stick_cal[10] << 8) & 0xF00 | factory_stick_cal[9];
			stick_cal_y_r[1] = (factory_stick_cal[11] << 4) | (factory_stick_cal[10] >> 4);
			stick_cal_x_r[0] = stick_cal_x_r[1] - ((factory_stick_cal[13] << 8) & 0xF00 | factory_stick_cal[12]);
			stick_cal_y_r[0] = stick_cal_y_r[1] - ((factory_stick_cal[14] << 4) | (factory_stick_cal[13] >> 4));
			stick_cal_x_r[2] = stick_cal_x_r[1] + ((factory_stick_cal[16] << 8) & 0xF00 | factory_stick_cal[15]);
			stick_cal_y_r[2] = stick_cal_y_r[1] + ((factory_stick_cal[17] << 4) | (factory_stick_cal[16] >> 4));
		}


		// if there is user calibration data:
		if ((user_stick_cal[0] | user_stick_cal[1] << 8) == 0xA1B2) {
			stick_cal_x_l[1] = (user_stick_cal[6] << 8) & 0xF00 | user_stick_cal[5];
			stick_cal_y_l[1] = (user_stick_cal[7] << 4) | (user_stick_cal[6] >> 4);
			stick_cal_x_l[0] = stick_cal_x_l[1] - ((user_stick_cal[9] << 8) & 0xF00 | user_stick_cal[8]);
			stick_cal_y_l[0] = stick_cal_y_l[1] - ((user_stick_cal[10] << 4) | (user_stick_cal[9] >> 4));
			stick_cal_x_l[2] = stick_cal_x_l[1] + ((user_stick_cal[3] << 8) & 0xF00 | user_stick_cal[2]);
			stick_cal_y_l[2] = stick_cal_y_l[1] + ((user_stick_cal[4] << 4) | (user_stick_cal[3] >> 4));
			//FormJoy::myform1->textBox_lstick_ucal->Text = String::Format(L"L Stick User:\r\nCenter X,Y: ({0:X3}, {1:X3})\r\nX: [{2:X3} - {4:X3}] Y: [{3:X3} - {5:X3}]",
			//stick_cal_x_l[1], stick_cal_y_l[1], stick_cal_x_l[0], stick_cal_y_l[0], stick_cal_x_l[2], stick_cal_y_l[2]);
		}
		else {
			//FormJoy::myform1->textBox_lstick_ucal->Text = L"L Stick User:\r\nNo calibration";
			//printf("no user Calibration data for left stick.\n");
		}

		if ((user_stick_cal[0xB] | user_stick_cal[0xC] << 8) == 0xA1B2) {
			stick_cal_x_r[1] = (user_stick_cal[14] << 8) & 0xF00 | user_stick_cal[13];
			stick_cal_y_r[1] = (user_stick_cal[15] << 4) | (user_stick_cal[14] >> 4);
			stick_cal_x_r[0] = stick_cal_x_r[1] - ((user_stick_cal[17] << 8) & 0xF00 | user_stick_cal[16]);
			stick_cal_y_r[0] = stick_cal_y_r[1] - ((user_stick_cal[18] << 4) | (user_stick_cal[17] >> 4));
			stick_cal_x_r[2] = stick_cal_x_r[1] + ((user_stick_cal[20] << 8) & 0xF00 | user_stick_cal[19]);
			stick_cal_y_r[2] = stick_cal_y_r[1] + ((user_stick_cal[21] << 4) | (user_stick_cal[20] >> 4));
			//FormJoy::myform1->textBox_rstick_ucal->Text = String::Format(L"R Stick User:\r\nCenter X,Y: ({0:X3}, {1:X3})\r\nX: [{2:X3} - {4:X3}] Y: [{3:X3} - {5:X3}]",
			//stick_cal_x_r[1], stick_cal_y_r[1], stick_cal_x_r[0], stick_cal_y_r[0], stick_cal_x_r[2], stick_cal_y_r[2]);
		}
		else {
			//FormJoy::myform1->textBox_rstick_ucal->Text = L"R Stick User:\r\nNo calibration";
			//printf("no user Calibration data for right stick.\n");
		}

		// get gyro / accelerometer calibration data:

		// factory calibration:

		// Acc cal origin position
		sensor_cal[0][0] = uint16_to_int16(factory_sensor_cal[0] | factory_sensor_cal[1] << 8);
		sensor_cal[0][1] = uint16_to_int16(factory_sensor_cal[2] | factory_sensor_cal[3] << 8);
		sensor_cal[0][2] = uint16_to_int16(factory_sensor_cal[4] | factory_sensor_cal[5] << 8);

		// Gyro cal origin position
		sensor_cal[1][0] = uint16_to_int16(factory_sensor_cal[0xC] | factory_sensor_cal[0xD] << 8);
		sensor_cal[1][1] = uint16_to_int16(factory_sensor_cal[0xE] | factory_sensor_cal[0xF] << 8);
		sensor_cal[1][2] = uint16_to_int16(factory_sensor_cal[0x10] | factory_sensor_cal[0x11] << 8);

		// user calibration:
		if ((user_sensor_cal[0x0] | user_sensor_cal[0x1] << 8) == 0xA1B2) {
			//FormJoy::myform1->textBox_6axis_ucal->Text = L"6-Axis User (XYZ):\r\nAcc:  ";
			//for (int i = 0; i < 0xC; i = i + 6) {
			//	FormJoy::myform1->textBox_6axis_ucal->Text += String::Format(L"{0:X4} {1:X4} {2:X4}\r\n      ",
			//		user_sensor_cal[i + 2] | user_sensor_cal[i + 3] << 8,
			//		user_sensor_cal[i + 4] | user_sensor_cal[i + 5] << 8,
			//		user_sensor_cal[i + 6] | user_sensor_cal[i + 7] << 8);
			//}
			// Acc cal origin position
			sensor_cal[0][0] = uint16_to_int16(user_sensor_cal[2] | user_sensor_cal[3] << 8);
			sensor_cal[0][1] = uint16_to_int16(user_sensor_cal[4] | user_sensor_cal[5] << 8);
			sensor_cal[0][2] = uint16_to_int16(user_sensor_cal[6] | user_sensor_cal[7] << 8);
			//FormJoy::myform1->textBox_6axis_ucal->Text += L"\r\nGyro: ";
			//for (int i = 0xC; i < 0x18; i = i + 6) {
			//	FormJoy::myform1->textBox_6axis_ucal->Text += String::Format(L"{0:X4} {1:X4} {2:X4}\r\n      ",
			//		user_sensor_cal[i + 2] | user_sensor_cal[i + 3] << 8,
			//		user_sensor_cal[i + 4] | user_sensor_cal[i + 5] << 8,
			//		user_sensor_cal[i + 6] | user_sensor_cal[i + 7] << 8);
			//}
			// Gyro cal origin position
			sensor_cal[1][0] = uint16_to_int16(user_sensor_cal[0xE] | user_sensor_cal[0xF] << 8);
			sensor_cal[1][1] = uint16_to_int16(user_sensor_cal[0x10] | user_sensor_cal[0x11] << 8);
			sensor_cal[1][2] = uint16_to_int16(user_sensor_cal[0x12] | user_sensor_cal[0x13] << 8);
		}
		else {
			//FormJoy::myform1->textBox_6axis_ucal->Text = L"\r\n\r\nUser:\r\nNo calibration";
		}

		// Use SPI calibration and convert them to SI acc unit
		acc_cal_coeff[0] = (float)(1.0 / (float)(16384 - uint16_to_int16(sensor_cal[0][0]))) * 4.0f  * 9.8f;
		acc_cal_coeff[1] = (float)(1.0 / (float)(16384 - uint16_to_int16(sensor_cal[0][1]))) * 4.0f  * 9.8f;
		acc_cal_coeff[2] = (float)(1.0 / (float)(16384 - uint16_to_int16(sensor_cal[0][2]))) * 4.0f  * 9.8f;

		// Use SPI calibration and convert them to SI gyro unit
		gyro_cal_coeff[0] = (float)(936.0 / (float)(13371 - uint16_to_int16(sensor_cal[1][0])) * 0.01745329251994);
		gyro_cal_coeff[1] = (float)(936.0 / (float)(13371 - uint16_to_int16(sensor_cal[1][1])) * 0.01745329251994);
		gyro_cal_coeff[2] = (float)(936.0 / (float)(13371 - uint16_to_int16(sensor_cal[1][2])) * 0.01745329251994);


		printf("Successfully initialized %s!\n", this->name.c_str());

		return 0;
	}
	
	void Joycon::init_usb() {

		this->bluetooth = false;

		unsigned char buf[0x400];
		memset(buf, 0, 0x400);

		// set blocking:
		// this insures we get the MAC Address
		hid_set_nonblocking(this->handle, 0);

		//Get MAC Left
		printf("Getting MAC...\n");
		memset(buf, 0x00, 0x40);
		buf[0] = 0x80;
		buf[1] = 0x01;
		hid_exchange(this->handle, buf, 0x2);

		if (buf[2] == 0x3) {
			printf("%s disconnected!\n", this->name.c_str());
		}
		else {
			printf("Found %s, MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", this->name.c_str(), buf[9], buf[8], buf[7], buf[6], buf[5], buf[4]);
		}

		// set non-blocking:
		//hid_set_nonblocking(jc->handle, 1);

		// Do handshaking
		printf("Doing handshake...\n");
		memset(buf, 0x00, 0x40);
		buf[0] = 0x80;
		buf[1] = 0x02;
		hid_exchange(this->handle, buf, 0x2);

		// Switch baudrate to 3Mbit
		printf("Switching baudrate...\n");
		memset(buf, 0x00, 0x40);
		buf[0] = 0x80;
		buf[1] = 0x03;
		hid_exchange(this->handle, buf, 0x2);

		//Do handshaking again at new baudrate so the firmware pulls pin 3 low?
		printf("Doing handshake...\n");
		memset(buf, 0x00, 0x40);
		buf[0] = 0x80;
		buf[1] = 0x02;
		hid_exchange(this->handle, buf, 0x2);

		//Only talk HID from now on
		printf("Only talk HID...\n");
		memset(buf, 0x00, 0x40);
		buf[0] = 0x80;
		buf[1] = 0x04;
		hid_exchange(this->handle, buf, 0x2);

		// Enable vibration
		printf("Enabling vibration...\n");
		memset(buf, 0x00, 0x400);
		buf[0] = 0x01; // Enabled
		send_subcommand(0x1, 0x48, buf, 1);

		// Enable IMU data
		printf("Enabling IMU data...\n");
		memset(buf, 0x00, 0x400);
		buf[0] = 0x01; // Enabled
		send_subcommand(0x1, 0x40, buf, 1);

		printf("Successfully initialized %s!\n", this->name.c_str());
	}

	void Joycon::deinit_usb() {
		unsigned char buf[0x40];
		memset(buf, 0x00, 0x40);

		//Let the Joy-Con talk BT again    
		buf[0] = 0x80;
		buf[1] = 0x05;
		hid_exchange(this->handle, buf, 0x2);
		printf("Deinitialized %s\n", this->name.c_str());
	}


	// calibrated sticks:
	// Credit to Hypersect (Ryan Juckett)
	// http://blog.hypersect.com/interpreting-analog-sticks/
	void Joycon::CalcAnalogStick() {

		if (this->left_right == 1) {
			CalcAnalogStick2(
				this->stick.CalX,
				this->stick.CalY,
				this->stick.x,
				this->stick.y,
				this->stick_cal_x_l,
				this->stick_cal_y_l);

		}
		else if (this->left_right == 2) {
			CalcAnalogStick2(
				this->stick.CalX,
				this->stick.CalY,
				this->stick.x,
				this->stick.y,
				this->stick_cal_x_r,
				this->stick_cal_y_r);

		}
		else if (this->left_right == 3) {
			CalcAnalogStick2(
				this->stick.CalX,
				this->stick.CalY,
				this->stick.x,
				this->stick.y,
				this->stick_cal_x_l,
				this->stick_cal_y_l);

			CalcAnalogStick2(
				this->stick2.CalX,
				this->stick2.CalY,
				this->stick2.x,
				this->stick2.y,
				this->stick_cal_x_r,
				this->stick_cal_y_r);
		}
	}


	void Joycon::CalcAnalogStick2
	(
		float &pOutX,       // out: resulting stick X value
		float &pOutY,       // out: resulting stick Y value
		uint16_t x,              // in: initial stick X value
		uint16_t y,              // in: initial stick Y value
		uint16_t x_calc[3],      // calc -X, CenterX, +X
		uint16_t y_calc[3]       // calc -Y, CenterY, +Y
	)
	{

		float x_f, y_f;
		// Apply Joy-Con center deadzone. 0xAE translates approx to 15%. Pro controller has a 10% () deadzone
		float deadZoneCenter = 0.15f;
		// Add a small ammount of outer deadzone to avoid edge cases or machine variety.
		float deadZoneOuter = 0.10f;

		// convert to float based on calibration and valid ranges per +/-axis
		x = clamp(x, x_calc[0], x_calc[2]);
		y = clamp(y, y_calc[0], y_calc[2]);
		if (x >= x_calc[1]) {
			x_f = (float)(x - x_calc[1]) / (float)(x_calc[2] - x_calc[1]);
		}
		else {
			x_f = -((float)(x - x_calc[1]) / (float)(x_calc[0] - x_calc[1]));
		}
		if (y >= y_calc[1]) {
			y_f = (float)(y - y_calc[1]) / (float)(y_calc[2] - y_calc[1]);
		}
		else {
			y_f = -((float)(y - y_calc[1]) / (float)(y_calc[0] - y_calc[1]));
		}

		// Interpolate zone between deadzones
		float mag = sqrtf(x_f*x_f + y_f*y_f);
		if (mag > deadZoneCenter) {
			// scale such that output magnitude is in the range [0.0f, 1.0f]
			float legalRange = 1.0f - deadZoneOuter - deadZoneCenter;
			float normalizedMag = min(1.0f, (mag - deadZoneCenter) / legalRange);
			float scale = normalizedMag / mag;
			pOutX = (x_f * scale);
			pOutY = (y_f * scale);
		}
		else {
			// stick is in the inner dead zone
			pOutX = 0.0f;
			pOutY = 0.0f;
		}
	}
	
	// SPI (@CTCaer):

	int Joycon::get_spi_data(uint32_t offset, const uint16_t read_len, uint8_t *test_buf) {
		int res;
		uint8_t buf[0x100];
		while (1) {
			memset(buf, 0, sizeof(buf));
			auto hdr = (brcm_hdr *)buf;
			auto pkt = (brcm_cmd_01 *)(hdr + 1);
			hdr->cmd = 1;
			hdr->rumble[0] = timing_byte;

			buf[1] = timing_byte;

			timing_byte++;
			if (timing_byte > 0xF) {
				timing_byte = 0x0;
			}
			pkt->subcmd = 0x10;
			pkt->offset = offset;
			pkt->size = read_len;

			for (int i = 11; i < 22; ++i) {
				buf[i] = buf[i + 3];
			}

			res = hid_write(handle, buf, sizeof(*hdr) + sizeof(*pkt));

			res = hid_read(handle, buf, sizeof(buf));

			if ((*(uint16_t*)&buf[0xD] == 0x1090) && (*(uint32_t*)&buf[0xF] == offset)) {
				break;
			}
		}
		if (res >= 0x14 + read_len) {
			for (int i = 0; i < read_len; i++) {
				test_buf[i] = buf[0x14 + i];
			}
		}

		return 0;
	}

	int Joycon::write_spi_data(uint32_t offset, const uint16_t write_len, uint8_t* test_buf) {
		int res;
		uint8_t buf[0x100];
		int error_writing = 0;
		while (1) {
			memset(buf, 0, sizeof(buf));
			auto hdr = (brcm_hdr *)buf;
			auto pkt = (brcm_cmd_01 *)(hdr + 1);
			hdr->cmd = 1;
			hdr->rumble[0] = timing_byte;
			timing_byte++;
			if (timing_byte > 0xF) {
				timing_byte = 0x0;
			}
			pkt->subcmd = 0x11;
			pkt->offset = offset;
			pkt->size = write_len;
			for (int i = 0; i < write_len; i++) {
				buf[0x10 + i] = test_buf[i];
			}
			res = hid_write(handle, buf, sizeof(*hdr) + sizeof(*pkt) + write_len);

			res = hid_read(handle, buf, sizeof(buf));

			if (*(uint16_t*)&buf[0xD] == 0x1180)
				break;

			error_writing++;
			if (error_writing == 125) {
				return 1;
			}
		}

		return 0;

	}


	//int write_spi_data(uint32_t offset, const uint16_t write_len, uint8_t* test_buf) {
	//	int res;
	//	uint8_t buf[0x100];
	//	int error_writing = 0;
	//	while (1) {
	//		memset(buf, 0, sizeof(buf));
	//		auto hdr = (brcm_hdr *)buf;
	//		auto pkt = (brcm_cmd_01 *)(hdr + 1);
	//		hdr->cmd = 1;
	//		hdr->timer = timing_byte & 0xF;
	//		timing_byte++;
	//		pkt->subcmd = 0x11;
	//		pkt->spi_read.offset = offset;
	//		pkt->spi_read.size = write_len;
	//		for (int i = 0; i < write_len; i++) {
	//			buf[0x10 + i] = test_buf[i];
	//		}

	//		res = hid_write(handle, buf, sizeof(*hdr) + sizeof(*pkt) + write_len);

	//		res = hid_read(handle, buf, sizeof(buf));
	//		if (*(uint16_t*)&buf[0xD] == 0x1180) {
	//			break;
	//		}
	//		error_writing++;
	//		if (error_writing == 125) {
	//			return 1;
	//		}
	//	}

	//	return 0;
	//}



	//int get_spi_data(uint32_t offset, const uint16_t read_len, uint8_t* test_buf) {
	//	int res;
	//	uint8_t buf[0x100];
	//	while (1) {
	//		memset(buf, 0, sizeof(buf));
	//		auto hdr = (brcm_hdr *)buf;
	//		auto pkt = (brcm_cmd_01 *)(hdr + 1);
	//		hdr->cmd = 1;
	//		hdr->timer = timing_byte & 0xF;
	//		timing_byte++;
	//		pkt->subcmd = 0x10;
	//		pkt->spi_read.offset = offset;
	//		pkt->spi_read.size = read_len;
	//		res = hid_write(handle, buf, sizeof(*hdr) + sizeof(*pkt));

	//		res = hid_read(handle, buf, sizeof(buf));
	//		if ((*(u16*)&buf[0xD] == 0x1090) && (*(uint32_t*)&buf[0xF] == offset)) {
	//			break;
	//		}
	//	}
	//	if (res >= 0x14 + read_len) {
	//		for (int i = 0; i < read_len; i++) {
	//			test_buf[i] = buf[0x14 + i];
	//		}
	//	}

	//	return 0;
	//}

	struct Settings {

		// Enabling this combines both JoyCons to a single vJoy Device(#1)
		// when combineJoyCons == false:
		// JoyCon(L) is mapped to vJoy Device #1
		// JoyCon(R) is mapped to vJoy Device #2
		// when combineJoyCons == true:
		// JoyCon(L) and JoyCon(R) are mapped to vJoy Device #1
		bool combineJoyCons = false;

		bool reverseX = false;// reverses joystick x (both sticks)
		bool reverseY = false;// reverses joystick y (both sticks)

		bool usingGrip = false;
		bool usingBluetooth = true;
		bool disconnect = false;

		// enables motion controls
		bool enableGyro = false;

		// gyroscope (mouse) sensitivity:
		float gyroSensitivityX = 150.0f;
		float gyroSensitivityY = 150.0f;


		// prefer the left joycon for gyro controls
		bool preferLeftJoyCon = false;

		// combo code to set key combination to disable gyroscope for quick turning in games. -1 to disable.
		int gyroscopeComboCode = 4;

		// toggles between two different toggle types
		// disabled = traditional toggle
		// enabled = while button(s) are held gyro is enabled
		bool quickToggleGyro = false;

		// inverts the above function
		bool invertQuickToggle = false;

		// for dolphin, mostly:
		bool dolphinPointerMode = false;

		// so that you don't rapidly toggle the gyro controls every frame:
		bool canToggleGyro = true;


		// enables 3D gyroscope visualizer
		bool gyroWindow = false;

		// plays a version of the mario theme by vibrating
		// the first JoyCon connected.
		bool marioTheme = false;

		// bool to restart the program
		bool restart = false;

		// auto start the program
		bool autoStart = false;

		// debug mode
		bool debugMode = false;

		// write debug to file:
		bool writeDebugToFile = false;

		// debug file:
		FILE* outputFile;

		// broadcast mode:
		bool broadcastMode = false;
		// where to connect:
		std::string host = "";
		// string to send:
		std::string controllerState = "";
		// write cast to file:
		bool writeCastToFile = false;

		// poll options:

		// force joycon to update when polled:
		bool forcePollUpdate = false;

		// times to poll per second per joycon:
		float pollsPerSec = 30.0f;

		// time to sleep (in ms) between polls:
		float timeToSleepMS = 4.0f;

		// version number
		std::string version = "1.07";

	} settings;

	struct Tracker {

		int var1 = 0;
		int var2 = 0;
		int counter1 = 0;

		float low_freq = 200.0f;
		float high_freq = 500.0f;

		float relX = 0;
		float relY = 0;

		float anglex = 0;
		float angley = 0;
		float anglez = 0;

//		glm::fquat quat = glm::angleAxis(0.0f, glm::vec3(1.0, 0.0, 0.0));

		// get current time
		//std::chrono::high_resolution_clock tNow;
		//std::chrono::steady_clock::time_point tPoll = std::chrono::high_resolution_clock::now();
		std::vector<std::chrono::steady_clock::time_point> tPolls;
		//Tracker(int value) : tPolls(100, std::chrono::high_resolution_clock::now()) {}
		//auto tSleepStart = std::chrono::high_resolution_clock::now();

		float previousPitch = 0;
	} tracker;



	void handle_input(Joycon *jc, uint8_t *packet, int len) {

		// bluetooth button pressed packet:
		if (packet[0] == 0x3F) {

			uint16_t old_buttons = jc->buttons;
			int8_t old_dstick = jc->dstick;

			jc->dstick = packet[3];
			// todo: get button states here aswell:
		}

		// input update packet:
		// 0x21 is just buttons, 0x30 includes gyro, 0x31 includes NFC (large packet size)
		if (packet[0] == 0x21 || packet[0] == 0x30 || packet[0] == 0x31) {

			// offset for usb or bluetooth data:
			/*int offset = settings.usingBluetooth ? 0 : 10;*/
			int offset = jc->bluetooth ? 0 : 10;

			uint8_t *btn_data = packet + offset + 3;

			// get button states:
			{
				uint16_t states = 0;
				uint16_t states2 = 0;

				// Left JoyCon:
				if (jc->left_right == 1) {
					states = (btn_data[1] << 8) | (btn_data[2] & 0xFF);
					// Right JoyCon:
				}
				else if (jc->left_right == 2) {
					states = (btn_data[1] << 8) | (btn_data[0] & 0xFF);
					// Pro Controller:
				}
				else if (jc->left_right == 3) {
					states = (btn_data[1] << 8) | (btn_data[2] & 0xFF);
					states2 = (btn_data[1] << 8) | (btn_data[0] & 0xFF);
				}

				jc->buttons = states;
				// Pro Controller:
				if (jc->left_right == 3) {
					jc->buttons2 = states2;

					// fix some non-sense the Pro Controller does
					// clear nth bit
					//num &= ~(1UL << n);
					jc->buttons &= ~(1UL << 9);
					jc->buttons &= ~(1UL << 10);
					jc->buttons &= ~(1UL << 12);
					jc->buttons &= ~(1UL << 14);

					jc->buttons2 &= ~(1UL << 8);
					jc->buttons2 &= ~(1UL << 11);
					jc->buttons2 &= ~(1UL << 13);
				}
			}

			// get stick data:
			uint8_t *stick_data = packet + offset;
			if (jc->left_right == 1) {
				stick_data += 6;
			}
			else if (jc->left_right == 2) {
				stick_data += 9;
			}

			uint16_t stick_x = stick_data[0] | ((stick_data[1] & 0xF) << 8);
			uint16_t stick_y = (stick_data[1] >> 4) | (stick_data[2] << 4);
			jc->stick.x = stick_x;
			jc->stick.y = stick_y;

			// use calibration data:
			jc->CalcAnalogStick();

			// pro controller:
			if (jc->left_right == 3) {
				stick_data += 6;
				uint16_t stick_x = stick_data[0] | ((stick_data[1] & 0xF) << 8);
				uint16_t stick_y = (stick_data[1] >> 4) | (stick_data[2] << 4);
				jc->stick.x = (int)(unsigned int)stick_x;
				jc->stick.y = (int)(unsigned int)stick_y;
				stick_data += 3;
				uint16_t stick_x2 = stick_data[0] | ((stick_data[1] & 0xF) << 8);
				uint16_t stick_y2 = (stick_data[1] >> 4) | (stick_data[2] << 4);
				jc->stick2.x = (int)(unsigned int)stick_x2;
				jc->stick2.y = (int)(unsigned int)stick_y2;

				// calibration data:
				jc->CalcAnalogStick();
			}

			jc->battery = (stick_data[1] & 0xF0) >> 4;
			////printf("JoyCon battery: %d\n", jc->battery);

			// Accelerometer:
			// Accelerometer data is absolute (m/s^2)
			{

				// get accelerometer X:
				jc->accel.x = (float)(uint16_to_int16(packet[13] | (packet[14] << 8 & 0xFF00))) * jc->acc_cal_coeff[0];

				atemp[0] = (int)(uint16_to_int16((packet[13])));
				atemp[1] = (int)(uint16_to_int16((packet[14])));

				// get accelerometer Y:
				jc->accel.y = (float)(uint16_to_int16(packet[15] | (packet[16] << 8 & 0xFF00))) * jc->acc_cal_coeff[1];

				atemp[2] = (int)(uint16_to_int16((packet[15])));
				atemp[3] = (int)(uint16_to_int16((packet[16])));

				// get accelerometer Z:
				jc->accel.z = (float)(uint16_to_int16(packet[17] | (packet[18] << 8 & 0xFF00))) * jc->acc_cal_coeff[2];

				atemp[4] = (int)(uint16_to_int16((packet[17])));
				atemp[5] = (int)(uint16_to_int16((packet[18])));

			}



			// Gyroscope:
			// Gyroscope data is relative (rads/s)
			{

				// get roll:
					jc->gyro.roll = (float)((uint16_to_int16(packet[19] | (packet[20] << 8) & 0xFF00)) - jc->sensor_cal[1][0]) * jc->gyro_cal_coeff[0];

				// get pitch:
					jc->gyro.pitch = (float)((uint16_to_int16(packet[21] | (packet[22] << 8) & 0xFF00)) - jc->sensor_cal[1][1]) * jc->gyro_cal_coeff[1];

				// get yaw:
					jc->gyro.yaw = (float)((uint16_to_int16(packet[23] | (packet[24] << 8) & 0xFF00)) - jc->sensor_cal[1][2]) * jc->gyro_cal_coeff[2];
			}

			// offsets:
			{
				jc->setGyroOffsets();

				jc->gyro.roll -= jc->gyro.offset.roll;
				jc->gyro.pitch -= jc->gyro.offset.pitch;
				jc->gyro.yaw -= jc->gyro.offset.yaw;

				//tracker.counter1++;
				//if (tracker.counter1 > 10) {
				//	tracker.counter1 = 0;
				//	//printf("%.3f %.3f %.3f\n", abs(jc->gyro.roll), abs(jc->gyro.pitch), abs(jc->gyro.yaw));
				//}
			}


			//hex_dump(gyro_data, 20);

			if (jc->left_right == 1) {
				//hex_dump(gyro_data, 20);
				//hex_dump(packet+12, 20);
				////printf("x: %f, y: %f, z: %f\n", tracker.anglex, tracker.angley, tracker.anglez);
				////printf("%04x\n", jc->stick.x);
				////printf("%f\n", jc->stick.CalX);
				////printf("%d\n", jc->gyro.yaw);
				////printf("%02x\n", jc->gyro.roll);
				////printf("%04x\n", jc->gyro.yaw);
				////printf("%04x\n", jc->gyro.roll);
				////printf("%f\n", jc->gyro.roll);
				////printf("%d\n", accelXA);
				////printf("%d\n", jc->buttons);
				////printf("%.4f\n", jc->gyro.pitch);
				////printf("%04x\n", accelX);
				////printf("%02x %02x\n", rollA, rollB);
			}

		}






		// handle button combos:
		{

			// press up, down, left, right, L, ZL to restart:
			if (jc->left_right == 1) {
				//if (jc->buttons == 207) {
				//	settings.restart = true;
				//}

				// remove this, it's just for rumble testing
				//uint8_t hfa2 = 0x88;
				//uint16_t lfa2 = 0x804d;

				//tracker.low_freq = clamp(tracker.low_freq, 41.0f, 626.0f);
				//tracker.high_freq = clamp(tracker.high_freq, 82.0f, 1252.0f);
				//
				//// down:
				//if (jc->buttons == 1) {
				//	tracker.high_freq -= 1;
				//	jc->rumble4(tracker.low_freq, tracker.high_freq, hfa2, lfa2);
				//}
				//// down:
				//if (jc->buttons == 2) {
				//	tracker.high_freq += 1;
				//	jc->rumble4(tracker.low_freq, tracker.high_freq, hfa2, lfa2);
				//}

				//// left:
				//if (jc->buttons == 8) {
				//	tracker.low_freq -= 1;
				//	jc->rumble4(tracker.low_freq, tracker.high_freq, hfa2, lfa2);
				//}
				//// right:
				//if (jc->buttons == 4) {
				//	tracker.low_freq += 1;
				//	jc->rumble4(tracker.low_freq, tracker.high_freq, hfa2, lfa2);
				//}

				//////printf("%i\n", jc->buttons);
				//////printf("%f\n", tracker.frequency);
				////printf("%f %f\n", tracker.low_freq, tracker.high_freq);
			}


			// left:
			if (jc->left_right == 1) {
				jc->btns.down = (jc->buttons & (1 << 0)) ? 1 : 0;
				jc->btns.up = (jc->buttons & (1 << 1)) ? 1 : 0;
				jc->btns.right = (jc->buttons & (1 << 2)) ? 1 : 0;
				jc->btns.left = (jc->buttons & (1 << 3)) ? 1 : 0;
				jc->btns.sr = (jc->buttons & (1 << 4)) ? 1 : 0;
				jc->btns.sl = (jc->buttons & (1 << 5)) ? 1 : 0;
				jc->btns.l = (jc->buttons & (1 << 6)) ? 1 : 0;
				jc->btns.zl = (jc->buttons & (1 << 7)) ? 1 : 0;
				jc->btns.minus = (jc->buttons & (1 << 8)) ? 1 : 0;
				jc->btns.stick_button = (jc->buttons & (1 << 11)) ? 1 : 0;
				jc->btns.capture = (jc->buttons & (1 << 13)) ? 1 : 0;


				if (settings.debugMode) {
					//printf("U: %d D: %d L: %d R: %d LL: %d ZL: %d SB: %d SL: %d SR: %d M: %d C: %d SX: %.5f SY: %.5f GR: %06d GP: %06d GY: %06d\n", \
														jc->btns.up, jc->btns.down, jc->btns.left, jc->btns.right, jc->btns.l, jc->btns.zl, jc->btns.stick_button, jc->btns.sl, jc->btns.sr, \
					jc->btns.minus, jc->btns.capture, (jc->stick.CalX + 1), (jc->stick.CalY + 1), (int)jc->gyro.roll, (int)jc->gyro.pitch, (int)jc->gyro.yaw);
				}
				if (settings.writeDebugToFile) {
					//fprintf(settings.outputFile, "U: %d D: %d L: %d R: %d LL: %d ZL: %d SB: %d SL: %d SR: %d M: %d C: %d SX: %.5f SY: %.5f GR: %06d GP: %06d GY: %06d\n", \
														jc->btns.up, jc->btns.down, jc->btns.left, jc->btns.right, jc->btns.l, jc->btns.zl, jc->btns.stick_button, jc->btns.sl, jc->btns.sr, \
					jc->btns.minus, jc->btns.capture, (jc->stick.CalX + 1), (jc->stick.CalY + 1), (int)jc->gyro.roll, (int)jc->gyro.pitch, (int)jc->gyro.yaw);
				}
			}

			// right:
			if (jc->left_right == 2) {
				jc->btns.y = (jc->buttons & (1 << 0)) ? 1 : 0;
				jc->btns.x = (jc->buttons & (1 << 1)) ? 1 : 0;
				jc->btns.b = (jc->buttons & (1 << 2)) ? 1 : 0;
				jc->btns.a = (jc->buttons & (1 << 3)) ? 1 : 0;
				jc->btns.sr = (jc->buttons & (1 << 4)) ? 1 : 0;
				jc->btns.sl = (jc->buttons & (1 << 5)) ? 1 : 0;
				jc->btns.r = (jc->buttons & (1 << 6)) ? 1 : 0;
				jc->btns.zr = (jc->buttons & (1 << 7)) ? 1 : 0;
				jc->btns.plus = (jc->buttons & (1 << 9)) ? 1 : 0;
				jc->btns.stick_button = (jc->buttons & (1 << 10)) ? 1 : 0;
				jc->btns.home = (jc->buttons & (1 << 12)) ? 1 : 0;


				if (settings.debugMode) {
					//printf("A: %d B: %d X: %d Y: %d RR: %d ZR: %d SB: %d SL: %d SR: %d P: %d H: %d SX: %.5f SY: %.5f GR: %06d GP: %06d GY: %06d\n", \
														jc->btns.a, jc->btns.b, jc->btns.x, jc->btns.y, jc->btns.r, jc->btns.zr, jc->btns.stick_button, jc->btns.sl, jc->btns.sr, \
					jc->btns.plus, jc->btns.home, (jc->stick.CalX + 1), (jc->stick.CalY + 1), (int)jc->gyro.roll, (int)jc->gyro.pitch, (int)jc->gyro.yaw);
				}
				if (settings.writeDebugToFile) {
					//fprintf(settings.outputFile, "A: %d B: %d X: %d Y: %d RR: %d ZR: %d SB: %d SL: %d SR: %d P: %d H: %d SX: %.5f SY: %.5f GR: %06d GP: %06d GY: %06d\n", \
														jc->btns.a, jc->btns.b, jc->btns.x, jc->btns.y, jc->btns.r, jc->btns.zr, jc->btns.stick_button, jc->btns.sl, jc->btns.sr, \
					jc->btns.plus, jc->btns.home, (jc->stick.CalX + 1), (jc->stick.CalY + 1), (int)jc->gyro.roll, (int)jc->gyro.pitch, (int)jc->gyro.yaw);
				}
			}

			// pro controller:
			if (jc->left_right == 3) {

				// left:
				jc->btns.down = (jc->buttons & (1 << 0)) ? 1 : 0;
				jc->btns.up = (jc->buttons & (1 << 1)) ? 1 : 0;
				jc->btns.right = (jc->buttons & (1 << 2)) ? 1 : 0;
				jc->btns.left = (jc->buttons & (1 << 3)) ? 1 : 0;
				jc->btns.sr = (jc->buttons & (1 << 4)) ? 1 : 0;
				jc->btns.sl = (jc->buttons & (1 << 5)) ? 1 : 0;
				jc->btns.l = (jc->buttons & (1 << 6)) ? 1 : 0;
				jc->btns.zl = (jc->buttons & (1 << 7)) ? 1 : 0;
				jc->btns.minus = (jc->buttons & (1 << 8)) ? 1 : 0;
				jc->btns.stick_button = (jc->buttons & (1 << 11)) ? 1 : 0;
				jc->btns.capture = (jc->buttons & (1 << 13)) ? 1 : 0;

				// right:
				jc->btns.y = (jc->buttons2 & (1 << 0)) ? 1 : 0;
				jc->btns.x = (jc->buttons2 & (1 << 1)) ? 1 : 0;
				jc->btns.b = (jc->buttons2 & (1 << 2)) ? 1 : 0;
				jc->btns.a = (jc->buttons2 & (1 << 3)) ? 1 : 0;
				jc->btns.sr = (jc->buttons2 & (1 << 4)) ? 1 : 0;
				jc->btns.sl = (jc->buttons2 & (1 << 5)) ? 1 : 0;
				jc->btns.r = (jc->buttons2 & (1 << 6)) ? 1 : 0;
				jc->btns.zr = (jc->buttons2 & (1 << 7)) ? 1 : 0;
				jc->btns.plus = (jc->buttons2 & (1 << 9)) ? 1 : 0;
				jc->btns.stick_button2 = (jc->buttons2 & (1 << 10)) ? 1 : 0;
				jc->btns.home = (jc->buttons2 & (1 << 12)) ? 1 : 0;


				if (settings.debugMode) {

					//printf("U: %d D: %d L: %d R: %d LL: %d ZL: %d SB: %d SL: %d SR: %d M: %d C: %d SX: %.5f SY: %.5f GR: %06d GP: %06d GY: %06d\n", \
														jc->btns.up, jc->btns.down, jc->btns.left, jc->btns.right, jc->btns.l, jc->btns.zl, jc->btns.stick_button, jc->btns.sl, jc->btns.sr, \
					jc->btns.minus, jc->btns.capture, (jc->stick.CalX + 1), (jc->stick.CalY + 1), (int)jc->gyro.roll, (int)jc->gyro.pitch, (int)jc->gyro.yaw);

//printf("A: %d B: %d X: %d Y: %d RR: %d ZR: %d SB: %d SL: %d SR: %d P: %d H: %d SX: %.5f SY: %.5f GR: %06d GP: %06d GY: %06d\n", \
					jc->btns.a, jc->btns.b, jc->btns.x, jc->btns.y, jc->btns.r, jc->btns.zr, jc->btns.stick_button2, jc->btns.sl, jc->btns.sr, \
					jc->btns.plus, jc->btns.home, (jc->stick2.CalX + 1), (jc->stick2.CalY + 1), (int)jc->gyro.roll, (int)jc->gyro.pitch, (int)jc->gyro.yaw);
				}

				if (settings.writeDebugToFile) {
					//fprintf(settings.outputFile, "U: %d D: %d L: %d R: %d LL: %d ZL: %d SB: %d SL: %d SR: %d M: %d C: %d SX: %.5f SY: %.5f GR: %06d GP: %06d GY: %06d\n", \
														jc->btns.up, jc->btns.down, jc->btns.left, jc->btns.right, jc->btns.l, jc->btns.zl, jc->btns.stick_button, jc->btns.sl, jc->btns.sr, \
					jc->btns.minus, jc->btns.capture, (jc->stick.CalX + 1), (jc->stick.CalY + 1), (int)jc->gyro.roll, (int)jc->gyro.pitch, (int)jc->gyro.yaw);

//fprintf(settings.outputFile, "A: %d B: %d X: %d Y: %d RR: %d ZR: %d SB: %d SL: %d SR: %d P: %d H: %d SX: %.5f SY: %.5f GR: %06d GP: %06d GY: %06d\n", \
					jc->btns.a, jc->btns.b, jc->btns.x, jc->btns.y, jc->btns.r, jc->btns.zr, jc->btns.stick_button2, jc->btns.sl, jc->btns.sr, \
					jc->btns.plus, jc->btns.home, (jc->stick2.CalX + 1), (jc->stick2.CalY + 1), (int)jc->gyro.roll, (int)jc->gyro.pitch, (int)jc->gyro.yaw);
				}

			}

		}
	}

	void Joycon_Initialize() {
		// Enumerate and print the HID devices on the system
		struct hid_device_info *devs, *cur_dev;

		res = hid_init();
		devs = hid_enumerate(JOYCON_VENDOR, 0x0);
		cur_dev = devs;
		while (cur_dev) {

			// identify by vendor:
			if (cur_dev->vendor_id == JOYCON_VENDOR) {

				// bluetooth, left / right joycon:
				if (cur_dev->product_id == JOYCON_L_BT || cur_dev->product_id == JOYCON_R_BT) {
					Joycon jc = Joycon(cur_dev);
					joycons.push_back(jc);
				}

				// pro controller:
				if (cur_dev->product_id == PRO_CONTROLLER) {
					Joycon jc = Joycon(cur_dev);
					joycons.push_back(jc);
				}

				// charging grip:
				if (cur_dev->product_id == JOYCON_CHARGING_GRIP) {
					Joycon jc = Joycon(cur_dev);
					settings.usingBluetooth = false;
					settings.combineJoyCons = true;
					joycons.push_back(jc);
				}

			}


			cur_dev = cur_dev->next;
		}
		hid_free_enumeration(devs);

		// init joycons:
		if (settings.usingGrip) {
			for (int i = 0; i < joycons.size(); ++i) {
				joycons[i].init_usb();
			}
		}
		else {
			for (int i = 0; i < joycons.size(); ++i) {
				joycons[i].init_bt();
			}
		}

		if (settings.combineJoyCons) {
			int counter = 0;
			for (int i = 0; i < joycons.size(); ++i) {
				joycons[i].vJoyNumber = (counter / 2) + 1;
				joycons[i].deviceNumber = (counter % 2 ? 1 : 0);
				counter++;
			}
		}
		else {
			for (int i = 0; i < joycons.size(); ++i) {
				joycons[i].vJoyNumber = i + 1;
				joycons[i].deviceNumber = 0;// left
			}
		}

		for (int i = 0; i < joycons.size(); ++i) {
			Joycon *jc = &joycons[i];
			// Player LED Enable
			memset(buf, 0x00, 0x40);
			if (i == 0) {
				buf[0] = 0x0 | 0x0 | 0x0 | 0x1;		// solid 1
			}
			if (i == 1) {
				if (settings.combineJoyCons) {
					buf[0] = 0x0 | 0x0 | 0x0 | 0x1; // solid 1
				}
				else if (!settings.combineJoyCons) {
					buf[0] = 0x0 | 0x0 | 0x2 | 0x0; // solid 2
				}
			}
			//buf[0] = 0x80 | 0x40 | 0x2 | 0x1; // Flash top two, solid bottom two
			//buf[0] = 0x8 | 0x4 | 0x2 | 0x1; // All solid
			//buf[0] = 0x80 | 0x40 | 0x20 | 0x10; // All flashing
			//buf[0] = 0x80 | 0x00 | 0x20 | 0x10; // All flashing except 3rd light (off)
			jc->send_subcommand(0x01, 0x30, buf, 1);
		}
		
	}
	void Joycon_Finalize() {

		for (int i = 0; i < joycons.size(); ++i) {
			buf[0] = 0x0; // disconnect
			joycons[i].send_subcommand(0x01, 0x06, buf, 1);
		}

		if (settings.usingGrip) {
			for (int i = 0; i < joycons.size(); ++i) {
				joycons[i].deinit_usb();
			}
		}
		// Finalize the hidapi library
		res = hid_exit();
	}

	void pollLoop() {

		// poll joycons:
		for (int i = 0; i < joycons.size(); ++i) {

			Joycon *jc = &joycons[i];

			// choose a random joycon to reduce bias / figure out the problem w/input lag:
			//Joycon *jc = &joycons[rand_range(0, joycons.size()-1)];

			if (!jc->handle) { continue; }


			if (settings.forcePollUpdate) {
				// set to be blocking:
				hid_set_nonblocking(jc->handle, 0);
			}
			else {
				// set to be non-blocking:
				hid_set_nonblocking(jc->handle, 1);
			}

			// get input:
			// time spent sleeping (0):
			//double timeSincePollMS = timeSincePoll.count() / 1000.0;

			//	if (timeSincePollMS > (1000.0 / settings.pollsPerSec)) {
			jc->send_command(0x1E, buf, 0);
			


			//hid_read(jc->handle, buf, 0x40);
			hid_read_timeout(jc->handle, buf, 0x40, 20);
			// get rid of queue:
			// if we force the poll to wait then the queue will never clear and will just freeze:
			//if (!settings.forcePollUpdate) {
			//	while (hid_read(jc->handle, buf, 0x40) > 0) {};
			//}

			//for (int i = 0; i < 100; ++i) {
			//	hid_read(jc->handle, buf, 0x40);
			//}

			handle_input(jc, buf, 0x40);

			if (GetJoyconDevice(0).btns.zl == 1) {
				joycons[0].accel.x = 0;
				joycons[0].accel.y = 0;
				joycons[0].accel.z = 0;
			}
		}

		// update vjoy:
		/*	for (int i = 0; i < joycons.size(); ++i) {
		updatevJoyDevice2(&joycons[i]);
		}

		// sleep:
		/*	if (settings.writeCastToFile) {
		veryAccurateSleep(settings.timeToSleepMS);
		}
		else {
		accurateSleep(settings.timeToSleepMS);
		}
		*/
		/*
		if (settings.broadcastMode && joycons.size() == 1) {
		Joycon *jc = &joycons[0];
		std::string newControllerState = "";


		if (jc->btns.up == 1 && jc->btns.left == 1) {
		newControllerState += "7";
		}
		else if (jc->btns.up && jc->btns.right == 1) {
		newControllerState += "1";
		}
		else if (jc->btns.down == 1 && jc->btns.left == 1) {
		newControllerState += "5";
		}
		else if (jc->btns.down == 1 && jc->btns.right == 1) {
		newControllerState += "3";
		}
		else if (jc->btns.up == 1) {
		newControllerState += "0";
		}
		else if (jc->btns.down == 1) {
		newControllerState += "4";
		}
		else if (jc->btns.left == 1) {
		newControllerState += "6";
		}
		else if (jc->btns.right == 1) {
		newControllerState += "2";
		}
		else {
		newControllerState += "8";
		}

		newControllerState += jc->btns.stick_button == 1 ? "1" : "0";
		newControllerState += jc->btns.l == 1 ? "1" : "0";
		newControllerState += jc->btns.zl == 1 ? "1" : "0";
		newControllerState += jc->btns.minus == 1 ? "1" : "0";
		newControllerState += jc->btns.capture == 1 ? "1" : "0";

		newControllerState += jc->btns.a == 1 ? "1" : "0";
		newControllerState += jc->btns.b == 1 ? "1" : "0";
		newControllerState += jc->btns.x == 1 ? "1" : "0";
		newControllerState += jc->btns.y == 1 ? "1" : "0";
		newControllerState += jc->btns.stick_button2 == 1 ? "1" : "0";
		newControllerState += jc->btns.r == 1 ? "1" : "0";
		newControllerState += jc->btns.zr == 1 ? "1" : "0";
		newControllerState += jc->btns.plus == 1 ? "1" : "0";
		newControllerState += jc->btns.home == 1 ? "1" : "0";

		int LX = ((jc->stick.CalX - 1.0) * 128) + 255;
		int LY = ((jc->stick.CalY - 1.0) * 128) + 255;
		int RX = ((jc->stick2.CalX - 1.0) * 128) + 255;
		int RY = ((jc->stick2.CalY - 1.0) * 128) + 255;

		newControllerState += " " + std::to_string(LX) + " " + std::to_string(LY) + " " + std::to_string(RX) + " " + std::to_string(RY);

		if (newControllerState != settings.controllerState) {
		settings.controllerState = newControllerState;
		//printf("%s\n", newControllerState);
		myClient.socket()->emit("sendControllerState", newControllerState);
		}

		if (settings.writeCastToFile) {
		//std::string filename = "C:\\Users\\Matt\\Desktop\\commands.txt";
		//FILE* outputFile = fopen(filename.c_str(), "w");
		//f//printf(outputFile, "%s\n", newControllerState);
		//fclose(outputFile);
		f//printf(settings.outputFile, "%s\n", newControllerState);
		}
		}

		if (settings.broadcastMode && joycons.size() == 2) {
		Joycon *jcL;
		Joycon *jcR;

		if (joycons[0].left_right == 1) {
		jcL = &joycons[0];
		jcR = &joycons[1];
		}
		else {
		jcL = &joycons[1];
		jcR = &joycons[0];
		}

		std::string newControllerState = "";


		if (jcL->btns.up == 1 && jcL->btns.left == 1) {
		newControllerState += "7";
		}
		else if (jcL->btns.up && jcL->btns.right == 1) {
		newControllerState += "1";
		}
		else if (jcL->btns.down == 1 && jcL->btns.left == 1) {
		newControllerState += "5";
		}
		else if (jcL->btns.down == 1 && jcL->btns.right == 1) {
		newControllerState += "3";
		}
		else if (jcL->btns.up == 1) {
		newControllerState += "0";
		}
		else if (jcL->btns.down == 1) {
		newControllerState += "4";
		}
		else if (jcL->btns.left == 1) {
		newControllerState += "6";
		}
		else if (jcL->btns.right == 1) {
		newControllerState += "2";
		}
		else {
		newControllerState += "8";
		}

		newControllerState += jcL->btns.stick_button == 1 ? "1" : "0";
		newControllerState += jcL->btns.l == 1 ? "1" : "0";
		newControllerState += jcL->btns.zl == 1 ? "1" : "0";
		newControllerState += jcL->btns.minus == 1 ? "1" : "0";
		newControllerState += jcL->btns.capture == 1 ? "1" : "0";

		newControllerState += jcR->btns.a == 1 ? "1" : "0";
		newControllerState += jcR->btns.b == 1 ? "1" : "0";
		newControllerState += jcR->btns.x == 1 ? "1" : "0";
		newControllerState += jcR->btns.y == 1 ? "1" : "0";
		newControllerState += jcR->btns.stick_button2 == 1 ? "1" : "0";
		newControllerState += jcR->btns.r == 1 ? "1" : "0";
		newControllerState += jcR->btns.zr == 1 ? "1" : "0";
		newControllerState += jcR->btns.plus == 1 ? "1" : "0";
		newControllerState += jcR->btns.home == 1 ? "1" : "0";

		int LX = ((jcL->stick.CalX - 1.0) * 128) + 255;
		int LY = ((jcL->stick.CalY - 1.0) * 128) + 255;
		int RX = ((jcR->stick.CalX - 1.0) * 128) + 255;
		int RY = ((jcR->stick.CalY - 1.0) * 128) + 255;

		newControllerState += " " + std::to_string(LX) + " " + std::to_string(LY) + " " + std::to_string(RX) + " " + std::to_string(RY);

		if (newControllerState != settings.controllerState) {
		settings.controllerState = newControllerState;
		//printf("%s\n", newControllerState);
		myClient.socket()->emit("sendControllerState", newControllerState);
		}

		if (settings.writeCastToFile) {
		//std::string filename = "C:\\Users\\Matt\\Desktop\\commands.txt";
		//FILE* outputFile = fopen(filename.c_str(), "w");
		//f//printf(outputFile, "%s\n", newControllerState);
		//fclose(outputFile);
		f//printf(settings.outputFile, "%s\n", newControllerState);
		}
		}*/
		/*
		if (settings.restart) {
		settings.restart = false;
		start();
		}*/
		
		for (int i = 0; i < joycons.size();i++) {
			accel[i] = D3DXVECTOR3(GetJoyconDevice(i).accel.x, GetJoyconDevice(i).accel.y, GetJoyconDevice(i).accel.z);
			jyro[i] = D3DXVECTOR3(GetJoyconDevice(i).gyro.pitch, GetJoyconDevice(i).gyro.roll, GetJoyconDevice(i).gyro.yaw);
		}
		DebugFont_Draw(800, 400, "res:%d", res);

	}

	//joycon構造体をすべて取得する（iはジョイコンの登録の数）
	Joycon GetJoyconDevice(int i) {
		return joycons[i];
	}

	//joyconのジャイロの値を取得
	D3DXVECTOR3 GetJyro(int i) {
		return jyro[i];
	}

	//joyconの加速度の値を取得
	D3DXVECTOR3 GetAccel(int i) {
		return accel[i];
	}

	//joyconの向きを取得
	int JoyconVector(int i) {
		return atemp[i];
	}

	void Rumble(int i) {
		joycons[i].rumble(400, 2);
	}