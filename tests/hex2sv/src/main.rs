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
  if *byte == 34u8
  {
    return "\\\"".to_string();
  }
  if *byte == 92u8
  {
    return "\\\\".to_string();
  }
  return format!("\\x{:0>2X}", byte);
}

fn is_safe_to_push(c: &char, prev_escaped: &bool, prev_nul: &bool) -> bool
{
  // "graphic" characters, except for backslash and double quote, ...
  c.is_ascii_graphic() && *c != '\\' && *c != '"'
  // unless the previous character was escaped and the current is a hex digit
  && (!c.is_ascii_hexdigit() || !prev_escaped)
  // .. or the last char was NUL and the character is an octal digit
  && (!c.is_digit(8) || !prev_nul)
}

fn hex_to_string_view(octets: &str) -> Result<String, String>
{
  let mut result = String::with_capacity(octets.len() / 3);

  let mut spliterator = octets.split(' ').enumerate();
  let mut previous_was_escaped = false;
  let mut previous_was_nul = false;
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
      previous_was_escaped = false;
      previous_was_nul = false;
    }
    else if index < 24
    {
      result.push_str(&to_sequence(&byte));
      previous_was_escaped = byte != 0u8;
      previous_was_nul = byte == 0u8;
    }
    else
    {
      let c = byte as char;
      if is_safe_to_push(&c, &previous_was_escaped, &previous_was_nul)
      {
        result.push(c);
        previous_was_escaped = false;
        previous_was_nul = false;
      }
      else
      {
        result.push_str(&to_sequence(&byte));
        previous_was_escaped = byte != 0u8;
        previous_was_nul = byte == 0u8;
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
  fn test_hex_to_string_view_modl()
  {
    let input = "4D 41 54 4F 57 02 00 00 00 00 00 00 26 F8 01 00 0A 50 25 00 12 00 01 00 45 44 49 44 15 00 53 68 61 64 65 72 54 65 73 74 73 53 68 61 64 65 72 42 6F 78 00 4D 4F 44 4C 1A 00 53 68 61 64 65 72 54 65 73 74 73 5C 53 68 61 64 65 72 42 6F 78 2E 6E 69 66 00";
    let expected: Result<String, String> = Ok("MATO\\x57\\x02\\0\\0\\0\\0\\0\\0\\x26\\xF8\\x01\\0\\x0A\\x50\\x25\\0\\x12\\0\\x01\\0EDID\\x15\\0ShaderTestsShaderBox\\0MODL\\x1A\\0ShaderTests\\\\ShaderBox.nif\\0".to_string());
    let actual = hex_to_string_view(input);
    assert_eq!(actual, expected);
  }

  #[test]
  fn test_is_safe_to_push_backslash()
  {
    assert_eq!(is_safe_to_push(&'\\', &false, &false), false);
    assert_eq!(is_safe_to_push(&'\\', &false, &true), false);
    assert_eq!(is_safe_to_push(&'\\', &true, &false), false);
    assert_eq!(is_safe_to_push(&'\\', &true, &true), false);
  }

  #[test]
  fn test_is_safe_to_push_hex()
  {
    assert_eq!(is_safe_to_push(&'9', &false, &false), true);
    assert_eq!(is_safe_to_push(&'9', &false, &true), true);
    assert_eq!(is_safe_to_push(&'9', &true, &false), false);
    assert_eq!(is_safe_to_push(&'9', &true, &true), false);

    assert_eq!(is_safe_to_push(&'A', &false, &false), true);
    assert_eq!(is_safe_to_push(&'A', &false, &true), true);
    assert_eq!(is_safe_to_push(&'A', &true, &false), false);
    assert_eq!(is_safe_to_push(&'A', &true, &true), false);

    assert_eq!(is_safe_to_push(&'a', &false, &false), true);
    assert_eq!(is_safe_to_push(&'a', &false, &true), true);
    assert_eq!(is_safe_to_push(&'a', &true, &false), false);
    assert_eq!(is_safe_to_push(&'a', &true, &true), false);

    assert_eq!(is_safe_to_push(&'E', &false, &false), true);
    assert_eq!(is_safe_to_push(&'E', &false, &true), true);
    assert_eq!(is_safe_to_push(&'E', &true, &false), false);
    assert_eq!(is_safe_to_push(&'E', &true, &true), false);

    assert_eq!(is_safe_to_push(&'e', &false, &false), true);
    assert_eq!(is_safe_to_push(&'e', &false, &true), true);
    assert_eq!(is_safe_to_push(&'e', &true, &false), false);
    assert_eq!(is_safe_to_push(&'e', &true, &true), false);

    assert_eq!(is_safe_to_push(&'F', &false, &false), true);
    assert_eq!(is_safe_to_push(&'F', &false, &true), true);
    assert_eq!(is_safe_to_push(&'F', &true, &false), false);
    assert_eq!(is_safe_to_push(&'F', &true, &true), false);

    assert_eq!(is_safe_to_push(&'f', &false, &false), true);
    assert_eq!(is_safe_to_push(&'f', &false, &true), true);
    assert_eq!(is_safe_to_push(&'f', &true, &false), false);
    assert_eq!(is_safe_to_push(&'f', &true, &true), false);
  }

  #[test]
  fn test_is_safe_to_push_non_hex()
  {
    assert_eq!(is_safe_to_push(&'G', &false, &false), true);
    assert_eq!(is_safe_to_push(&'G', &false, &true), true);
    assert_eq!(is_safe_to_push(&'G', &true, &false), true);
    assert_eq!(is_safe_to_push(&'G', &true, &true), true);

    assert_eq!(is_safe_to_push(&'g', &false, &false), true);
    assert_eq!(is_safe_to_push(&'g', &false, &true), true);
    assert_eq!(is_safe_to_push(&'g', &true, &false), true);
    assert_eq!(is_safe_to_push(&'g', &true, &true), true);
  }

  #[test]
  fn test_is_safe_to_push_octal()
  {
    assert_eq!(is_safe_to_push(&'0', &false, &false), true);
    assert_eq!(is_safe_to_push(&'0', &false, &true), false);
    assert_eq!(is_safe_to_push(&'0', &true, &false), false);
    assert_eq!(is_safe_to_push(&'0', &true, &true), false);

    assert_eq!(is_safe_to_push(&'1', &false, &false), true);
    assert_eq!(is_safe_to_push(&'1', &false, &true), false);
    assert_eq!(is_safe_to_push(&'1', &true, &false), false);
    assert_eq!(is_safe_to_push(&'1', &true, &true), false);

    assert_eq!(is_safe_to_push(&'5', &false, &false), true);
    assert_eq!(is_safe_to_push(&'5', &false, &true), false);
    assert_eq!(is_safe_to_push(&'5', &true, &false), false);
    assert_eq!(is_safe_to_push(&'5', &true, &true), false);

    assert_eq!(is_safe_to_push(&'6', &false, &false), true);
    assert_eq!(is_safe_to_push(&'6', &false, &true), false);
    assert_eq!(is_safe_to_push(&'6', &true, &false), false);
    assert_eq!(is_safe_to_push(&'6', &true, &true), false);

    assert_eq!(is_safe_to_push(&'7', &false, &false), true);
    assert_eq!(is_safe_to_push(&'7', &false, &true), false);
    assert_eq!(is_safe_to_push(&'7', &true, &false), false);
    assert_eq!(is_safe_to_push(&'7', &true, &true), false);
  }

  #[test]
  fn test_is_safe_to_push_nul()
  {
    assert_eq!(is_safe_to_push(&'\0', &false, &false), false);
    assert_eq!(is_safe_to_push(&'\0', &false, &true), false);
    assert_eq!(is_safe_to_push(&'\0', &true, &false), false);
    assert_eq!(is_safe_to_push(&'\0', &true, &true), false);
  }

  #[test]
  fn test_is_safe_to_push_quote()
  {
    assert_eq!(is_safe_to_push(&'"', &false, &false), false);
    assert_eq!(is_safe_to_push(&'"', &false, &true), false);
    assert_eq!(is_safe_to_push(&'"', &true, &false), false);
    assert_eq!(is_safe_to_push(&'"', &true, &true), false);
  }

  #[test]
  fn test_to_sequence()
  {
    assert_eq!(to_sequence(&0), "\\0");
    assert_eq!(to_sequence(&10), "\\x0A");
    assert_eq!(to_sequence(&34), "\\\"");
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
