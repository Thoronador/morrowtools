/*
 -------------------------------------------------------------------------------
    This file is part of hex2sv.

    Copyright (C) 2021  Dirk Stolle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

use std::io::Read;

fn to_u8(s: &str) -> u8
{
  u8::from_str_radix(s, 16).unwrap()
}

fn to_sequence(byte: &u8) -> String
{
  if *byte == 0u8
  {
    return "\\0".to_string();
  }
  if *byte == 92u8
  {
    return "\\\\".to_string();
  }
  return format!("\\x{:0>2X}", byte);
}

fn is_safe_to_push(c: &char, s: &str) -> bool
{
  !c.is_ascii_hexdigit()
  || (s.chars().rev().nth(3) != Some('\\')
  && s.chars().rev().nth(2) != Some('x'))
}

fn hex_to_string_view(octets: &str) -> Result<String, String>
{
  let mut result = String::with_capacity(octets.len() / 3);

  let mut spliterator = octets.split(' ').enumerate();
  loop
  {
    let next = spliterator.next();
    if next.is_none()
    {
      break;
    }
    let (index, val) = next.unwrap();
    if val.len() != 2
    {
      return Err(format!("Value '{}' at index {} does not consist of two characters!", val, index));
    }

    let byte = to_u8(val);
    if index < 4
    {
      result.push(byte as char);
    }
    else if index < 24
    {
      result.push_str(&to_sequence(&byte));
    }
    else
    {
      let c = byte as char;
      if c.is_ascii_graphic() && c != '\\' && is_safe_to_push(&c, &result)
      {
        result.push(c);
      }
      else
      {
        result.push_str(&to_sequence(&byte));
      }
    }
  }

  Ok(result)
}

fn main() {
    let mut input = String::new();
    if let Err(e) = std::io::stdin().read_to_string(&mut input)
    {
        eprintln!("Error: Could not read from standard input! {}", e);
        return;
    }
    let input = input.trim();
    if input.is_empty()
    {
        eprintln!("Error: Input is empty!");
        return;
    }

    let sv = hex_to_string_view(input);
    if let Err(e) = sv
    {
      eprintln!("Error: {}", e);
      return;
    }
    println!("const auto data = \"{}\"sv;", sv.unwrap());
}


#[cfg(test)]
mod tests
{
  use super::*;

  #[test]
  fn test_hex_to_string_view()
  {
    let input = "41 41 43 54 11 00 00 00 00 00 00 00 02 30 01 00 19 4B 0E 00 0F 00 01 00 45 44 49 44 0B 00 41 63 74 69 6F 6E 49 64 6C 65 00";
    let expected: Result<String, String> = Ok("AACT\\x11\\0\\0\\0\\0\\0\\0\\0\\x02\\x30\\x01\\0\\x19\\x4B\\x0E\\0\\x0F\\0\\x01\\0EDID\\x0B\\0ActionIdle\\0".to_string());
    let actual = hex_to_string_view(input);
    assert_eq!(actual, expected);
  }

  #[test]
  fn test_to_sequence()
  {
    assert_eq!(to_sequence(&0), "\\0");
    assert_eq!(to_sequence(&10), "\\x0A");
    assert_eq!(to_sequence(&42), "\\x2A");
    assert_eq!(to_sequence(&81), "\\x51");
    assert_eq!(to_sequence(&255), "\\xFF");
  }

  #[test]
  fn test_to_u8()
  {
    assert_eq!(to_u8("00"), 0u8);
    assert_eq!(to_u8("0C"), 12u8);
    assert_eq!(to_u8("42"), 66u8);
    assert_eq!(to_u8("FF"), 255u8);
  }
}
