import { initializeApp, FirebaseApp } from "firebase/app";
import { getAnalytics, Analytics } from "firebase/analytics";
import { makeAutoObservable } from "mobx";

const firebaseConfig = {
  apiKey: "AIzaSyD35dn4IMvA7-Tul_OPeBGerHPHJfqypSk",
  authDomain: "ribbit-network.firebaseapp.com",
  projectId: "ribbit-network",
  storageBucket: "ribbit-network.appspot.com",
  messagingSenderId: "56492711430",
  appId: "1:56492711430:web:6a5d17592974fa06492d87",
  measurementId: "G-METWMB8750",
};

class Core {
  app: FirebaseApp;
  analytics: Analytics;

  sensorData: SensorData[] = [];

  constructor() {
    this.app = initializeApp(firebaseConfig);
    this.analytics = getAnalytics(this.app);

    makeAutoObservable(this);

    this.getMap();
  }

  async getMap() {
    const response = await fetch('https://us-central1-ribbit-network.cloudfunctions.net/getSensorData')

    this.sensorData = await response.json()
  }
}

export default new Core();

export interface SensorData {
  co2: number;
  host: string;
  lat: number;
  lon: number;
  result: string;
  table: number;
  _time: string;
}
