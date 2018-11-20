#ifndef JOYCON_H_
#define JOYCON_H_

#include <bitset>
#include <vector>
#include <d3dx9.h>
#include "hidapi.h"

#define JOYCON_VENDOR 0x057e
#define JOYCON_L_BT 0x2006
#define JOYCON_R_BT 0x2007
#define PRO_CONTROLLER 0x2009
#define JOYCON_CHARGING_GRIP 0x200e
#define L_OR_R(lr) (lr == 1 ? 'L' : (lr == 2 ? 'R' : '?'))


class Joycon {

public:
	hid_device *handle;
	wchar_t *serial;


	std::string name;

	int deviceNumber;// left(0) or right(1) vjoy
	int vJoyNumber;// vjoy device number / device group number

	bool bluetooth;

	int left_right = 0;// 1: left joycon, 2: right joycon, 3: pro controller

	uint16_t buttons = 0;
	uint16_t buttons2 = 0;// for pro controller


	struct btn_states {
		// left:
		int up = 0;
		int down = 0;
		int left = 0;
		int right = 0;
		int l = 0;
		int zl = 0;
		int minus = 0;
		int capture = 0;

		// right:
		int a = 0;
		int b = 0;
		int x = 0;
		int y = 0;
		int r = 0;
		int zr = 0;
		int plus = 0;
		int home = 0;

		// shared:
		int sl = 0;
		int sr = 0;
		int stick_button = 0;

		// pro controller:
		int stick_button2 = 0;// pro controller

	} btns;

	int8_t dstick;
	uint8_t battery;

	int global_count;

	struct Stick {
		uint16_t x = 0;
		uint16_t y = 0;
		float CalX = 0;
		float CalY = 0;
	};

	Stick stick;
	Stick stick2;// Pro Controller

	struct Gyroscope {
		// relative:
		float pitch = 0;
		float yaw = 0;
		float roll = 0;

		struct Offset {
			int n = 0;

			// absolute:
			float pitch = 0;
			float yaw = 0;
			float roll = 0;
		} offset;
	} gyro;

	struct Accelerometer {
		float prevX = 0;
		float prevY = 0;
		float prevZ = 0;
		float x = 0;
		float y = 0;
		float z = 0;
	} accel;


	// calibration data:

	struct brcm_hdr {
		uint8_t cmd;
		uint8_t rumble[9];
	};

	//struct brcm_cmd_01 {
	//	uint8_t subcmd;
	//	union {

	//		struct {
	//			uint32_t offset;
	//			uint8_t size;
	//		} spi_read;

	//		struct {
	//			uint32_t address;
	//		} hax_read;
	//	};
	//};

	struct brcm_cmd_01 {
		uint8_t subcmd;
		uint32_t offset;
		uint8_t size;
	};

	int timing_byte = 0x0;

	//struct brcm_hdr {
	//	uint8_t cmd;
	//	uint8_t timer;
	//	uint8_t rumble_l[4];
	//	uint8_t rumble_r[4];
	//};

	//struct brcm_cmd_01 {
	//	uint8_t subcmd;
	//	union {
	//		struct {
	//			uint32_t offset;
	//			uint8_t size;
	//		} spi_read;

	//		struct {
	//			uint8_t arg1;
	//			uint8_t arg2;
	//		} subcmd_arg;
	//	};
	//};

	//// Used to order the packets received in Joy-Con internally. Range 0x0-0xF.
	//uint8_t timing_byte = 0x0;



	float acc_cal_coeff[3];
	float gyro_cal_coeff[3];
	float cal_x[1] = { 0.0f };
	float cal_y[1] = { 0.0f };

	bool has_user_cal_stick_l = false;
	bool has_user_cal_stick_r = false;
	bool has_user_cal_sensor = false;

	unsigned char factory_stick_cal[0x12];
	unsigned char user_stick_cal[0x16];
	unsigned char sensor_model[0x6];
	unsigned char stick_model[0x24];
	unsigned char factory_sensor_cal[0x18];
	unsigned char user_sensor_cal[0x1A];
	uint16_t factory_sensor_cal_calm[0xC];
	uint16_t user_sensor_cal_calm[0xC];
	int16_t sensor_cal[0x2][0x3];
	uint16_t stick_cal_x_l[0x3];
	uint16_t stick_cal_y_l[0x3];
	uint16_t stick_cal_x_r[0x3];
	uint16_t stick_cal_y_r[0x3];

	Joycon(struct hid_device_info *dev);
	~Joycon();
	void hid_exchange(hid_device *handle, unsigned char *buf, int len);
	void send_command(int command, uint8_t *data, int len);
	void send_subcommand(int command, int subcommand, uint8_t *data, int len);
	void rumble(int frequency, int intensity);
	void rumble2(uint16_t hf, uint8_t hfa, uint8_t lf, uint16_t lfa);
	void rumble3(float frequency, uint8_t hfa, uint16_t lfa);
	void rumble4(float real_LF, float real_HF, uint8_t hfa, uint16_t lfa);
	void rumble_freq(uint16_t hf, uint8_t hfa, uint8_t lf, uint16_t lfa);
	void setGyroOffsets();
	int init_bt();
	void init_usb();
	void deinit_usb();
	void CalcAnalogStick();
	void CalcAnalogStick2(
		float &pOutX,       // out: resulting stick X value
		float &pOutY,       // out: resulting stick Y value
		uint16_t x,              // in: initial stick X value
		uint16_t y,              // in: initial stick Y value
		uint16_t x_calc[3],      // calc -X, CenterX, +X
		uint16_t y_calc[3]);       // calc -Y, CenterY, +Y	


	int get_spi_data(uint32_t offset, const uint16_t read_len, uint8_t *test_buf);
	int write_spi_data(uint32_t offset, const uint16_t write_len, uint8_t* test_buf);

};

void Joycon_Initialize();
void Joycon_Finalize();
void pollLoop();
Joycon GetJoyconDevice(int i);
D3DXVECTOR3 GetAccel(int i);
D3DXVECTOR3 GetJyro(int i);
int JoyconVector(int i);
void Rumble(int i);

#endif