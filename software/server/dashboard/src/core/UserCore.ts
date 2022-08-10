import { makeAutoObservable, toJS } from "mobx";
import { core } from "./core";
import { UserProfile } from "./types";
import {
  getDocs,
  getDoc,
  collection,
  doc,
  DocumentReference,
  DocumentData,
  updateDoc,
  setDoc,
} from "firebase/firestore";
import firebase from "firebase/compat";

class UserCore {
  public userRole?: string | null = null;

  public activeUser?: firebase.User;
  public userProfile?: UserProfile;

  public updateUser?: any;
  public userProfileReference?: DocumentReference<DocumentData>;

  authCheckComplete = false;

  constructor() {
    makeAutoObservable(this);
    this.authCheck();
  }

  /**
   * Authentication
   * */
  authCheck = () =>
    core.auth.onAuthStateChanged((user) => {
      this.authCheckComplete = true;
      if (user) {
        // @ts-ignore
        this.activeUser = user;
        // @ts-ignore
        this.subscribeToUserProfile(user);
      } else {
      }
    });

  async subscribeToUserProfile(user: firebase.User) {
    this.userProfileReference = doc(core.firestore, `users/${user.uid}`);

    this.userProfile = (
      await getDoc(this.userProfileReference)
    ).data() as UserProfile;

    if (this.userProfile) {
      this.updateUser = () =>
        // @ts-ignore
        updateDoc(this.userProfileReference!, toJS(this.userProfile));
    } else {
      // New user
      const userData: UserProfile = {
        uid: user.uid,
        email: user.email!,
        displayName: user.displayName!,
        phoneNumber: user.phoneNumber!,
        avatarUrl: user.photoURL!,

        verified: false,
      };

      setDoc(this.userProfileReference, userData);

      this.userProfile = userData;
    }
  }

  doLogout = () =>
    core.auth.signOut().then(() => {
      this.activeUser = undefined;
      this.userProfile = undefined;
    });
}

export const userCore = new UserCore();
