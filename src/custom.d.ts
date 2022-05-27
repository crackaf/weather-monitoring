declare module "*.svg" {
  const content: string;
  export = content;
}

declare module "*.png" {
  const value: any;
  export = value;
}

declare module "*.jpg" {
  const value: any;
  export = value;
}

declare module "react-fancy-countdown" {
  const value: { SlideCountdown: any };
  export = value;
}
