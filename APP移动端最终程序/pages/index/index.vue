<template>
	<view class="wrap">
		<view class="dev-area">

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">温度</view>
					<image class="dev-logo" src="../../static/temp.png" mode=""></image>
				</view>
				<view class="dev-data">{{temp}} ℃</view>
			</view>
			<view class="dev-cart">
				<view class="">
					<view class="dev-name">湿度</view>
					<image class="dev-logo" src="../../static/humi.png" mode=""></image>
				</view>
				<view class="dev-data">{{humi}} ℃</view>
			</view>
			<view class="dev-cart">
				<view class="">
					<view class="dev-name">台灯</view>
					<image class="dev-logo" src="../../static/lamp.png" mode=""></image>
				</view>
				<switch :checked="led0" @change="onLedSwitch" color="#2b9939"/>
			</view>
			
			<<!-- view class="dev-cart">
				<view class="">
					<view class="dev-name">卧室灯</view>
					<image class="dev-logo" src="../../static/lamp.png" mode=""></image>
				</view>
				<switch :checked="led1" @change="onLedSwitch" color="#2b9939"/>
			</view> -->
			<view class="dev-cart">
				<view class="">
					其他
				</view>
			</view>

		</view>
	</view>

</template>

<script>
	const {
		createCommonToken
	} = require('@/key.js')
	export default {
		data() {
			return {
				temp: '',
				humi: '',
				led0: true,
				token: '',
			}
		},
		onLoad() {
			const params = {
				author_key: 'LVNcRUBmce85vqq9IdQKJile5XNZaqvD/EvQIoA/P0/Vl69MJDrRhFZGDRG/7GP8LIdV8fErHKCB5qjtUADvZg==',
				version: '2022-05-01',
				user_id: '268184',
			}
			this.token = createCommonToken(params);
		},
		onShow() {
			this.fetchDevData();
			setInterval(()=>{
				this.fetchDevData();
			},3000)
		},
		methods: {
			fetchDevData() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property', //接口地址。
					method: 'GET',
					data: {
						product_id: '0vF205coZ6',
						device_name: 'Environmental_Monitoring'
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: (res) => {
						console.log(res.data);
						this.temp = res.data.data[5].value;
						this.humi = res.data.data[0].value;
						this.led0 = res.data.data[1].value == true;
						// this.led1 = res.data.data[2].value == true;

					}
				});
			},
			onLedSwitch(event) {
				console.log(event.detail.value);
				let value = event.detail.value;
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property', //仅为示例，并非真实接口地址。
					method: 'POST',
					data: {
						product_id: '0vF205coZ6',
						device_name: 'Environmental_Monitoring',
						params: {
							"led0": value
							// "led1": value
						}
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: () => {
						console.log('LED ' + (value ? 'ON' : 'OFF') + ' !');
					}
				});
			}

		}
	}
</script>

<style>
	.wrap {
		padding: 30rpx;
	}

	.dev-area {
		display: flex;
		justify-content: space-between;
		flex-wrap: wrap;
	}

	.dev-cart {
		height: 150rpx;
		width: 320rpx;
		border-radius: 30rpx;
		margin-top: 30rpx;
		display: flex;
		justify-content: space-around;
		align-items: center;
		box-shadow: 0 0 15rpx #ccc;
	}

	.dev-name {
		font-size: 20rpx;
		text-align: center;
		color: #6d6d6d;
	}

	.dev-logo {
		width: 70rpx;
		height: 70rpx;
		margin-top: 10rpx;
	}

	.dev-data {
		font-size: 50rpx;
		color: #6d6d6d;
	}

</style>