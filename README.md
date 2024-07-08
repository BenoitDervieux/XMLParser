# XML Parser Project

**Date of writing:** July the 8th 2024

This XML parser is a beginning of an XML parser that I did for a personal project for [www.lesklights.com](http://www.lesklights.com).

## Intend

The project intends to fetch and write data from an `.xml` document that I will place inside the microcontroller and that will hold important information such as serial number, user's name, wifi credentials, number of lights, and so on.

## Use

The parser is made only for this document and for some methods. This has not necessarily the pretension to be used for any other application as of now. The parser will fetch and write information from a `.xml` document with only node, subnode, and text fields. No more.

## Improvements

- Comply with [RFC 7303](https://datatracker.ietf.org/doc/html/rfc7303) documentation
- Send format errors
- Fetch and treat the attributes
- Others I can't think of right now

## Inspiration

I took inspiration from [https://github.com/jonahisadev/littlexml/blob/master/lxml.h](https://github.com/jonahisadev/littlexml/blob/master/lxml.h). I started to follow his YouTube tutorial first: [https://www.youtube.com/watch?v=kPFYfTvMRs8&t=3898s](https://www.youtube.com/watch?v=kPFYfTvMRs8&t=3898s), but then it went into a direction I didn't like even if I thought his way of coding was great. So I decided to do my own implementation but still by mimicking his best practices.
