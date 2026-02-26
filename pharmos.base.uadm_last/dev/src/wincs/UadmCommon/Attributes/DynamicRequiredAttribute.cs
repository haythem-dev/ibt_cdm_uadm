using System.ComponentModel.DataAnnotations;

namespace UadmCommon.Attributes
{
    public class DynamicRequiredOnHideAttribute : ValidationAttribute
    {
        private readonly string _conditionProperty;

        /// <summary>
        /// DynamicRequiredAttribute
        /// </summary>
        /// <param name="conditionProperty"></param>
        public DynamicRequiredOnHideAttribute(string conditionProperty)
        {
            _conditionProperty = conditionProperty;
        }

        /// <summary>
        /// IsValid
        /// </summary>
        /// <param name="value"></param>
        /// <param name="validationContext"></param>
        /// <returns></returns>
        protected override ValidationResult IsValid(object value, ValidationContext validationContext)
        {
            System.Reflection.PropertyInfo conditionProperty = validationContext.ObjectType.GetProperty(_conditionProperty);
            if (conditionProperty == null)
            {
                return new ValidationResult($"Property '{_conditionProperty}' not found.");
            }

            bool isHide = (bool)conditionProperty.GetValue(validationContext.ObjectInstance);

            if (!isHide && string.IsNullOrEmpty(value?.ToString()))
            {
                return new ValidationResult($"{validationContext.DisplayName} is required.");
            }

            return ValidationResult.Success;
        }
    }
}